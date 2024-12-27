#pragma once

#include "../def.h"
#include "../Vector.h"
#include "../openGL/shader.h"
#include "../Pair.h"
#include "../Flag.h"
#include "../Debug.h"
#include "../File.h"

#include <stdio.h>
#include <stdlib.h>
#include <unordered_map>
#include <functional>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#define GRAY_VEC4(gray_scale)  vec4(vec3(gray_scale), 1.0f)

#define WHITE_VEC4 vec4(1.0f)
#define BLACK_VEC4 vec4(vec3(0.0f), 1.0f)
#define RED_VEC4   vec4(1.0f, 0.0f, 0.0f, 1.0f)
#define GREEN_VEC4 vec4(0.0f, 1.0f, 0.0f, 1.0f)
#define BLUE_VEC4  vec4(0.0f, 0.0f, 1.0f, 1.0f)

#define WHITE_VEC3 vec3(1.0f)
#define BLACK_VEC3 vec3(0.0f)
#define GREY_VEC3(scale) vec3(scale)

#define glApp __glewApp::instance()
#define ELEMENT_FLAG_SIZE 16
#define GL_APP_FLAG_SIZE 8
#define VERTICES_SIZE 4
#define INDICES_SIZE 6

/* Define the vertices of a square (centered at the origin) */
__inline__ const vec2 vertices[] = {
  /* Pos. */   
  vec2(0.0f, 0.0f), /* Bottom-left */
  vec2(1.0f, 0.0f), /* Bottom-right */
  vec2(1.0f, 1.0f), /* Top-right */
  vec2(0.0f, 1.0f), /* Top-left */
};
__inline__ const Uint indices[] = {
  0, 1, 2, /* First triangle */
  2, 3, 0  /* Second triangle */
};

enum /* 'Element' Flag`s. */ {
  ALIGN_SIZE_WIDTH,
  ALIGN_SIZE_HEIGHT,
  ALIGN_POS_WIDTH,
  IN_ANIMATION,
  IS_BUTTON,
  BUTTON_PRESSED,
  BUTTON_DISABLED,
  DELETE_ELEMENT,
  DO_NOT_PREDUSE_LEAVE_EVENT_ON_CHILDREN_ENTER,
  PARENT_ALWAYS_OVER_CHILDREN,
  DROPDOWN_MENU_LEFT_SIDE,
  DROPDOWN_MENU_RIGHT_SIDE,
  DROPDOWN_MENU_TOP_SIDE,
  DROPDOWN_MENU_DOWN_SIDE,
  ELEMENT_MOUSE_INSIDE,
  #define ELEMENT_MOUSE_INSIDE ELEMENT_MOUSE_INSIDE
};
enum /* '__glewApp' Flag`s. */ {
  GL_APP_RUNNING,
  GL_HAS_MAIN_LOOP_LAMBDA,
  GLAPP_HAS_ACTIVE_FONT,
  #define GLAPP_HAS_ACTIVE_FONT GLAPP_HAS_ACTIVE_FONT
};

struct Character {
	Uint textureID; /* ID handle for the glyph texture. */
	ivec2 Size;			/* Size of glyph. */
	ivec2 Bearing;	/* Offset from baseline to left/top of glyph. */
	Uint Advance;		/* Offset to advance to next glyph. */
};

struct Font {
  std::map<char, Character> characters;
  vec2 size;

  /* Load a font from a path.  Setting fontw to zero means freetype dynamicly handles width based on height. */
  int load_font(const char *path, Uint fonth, Uint fontw = 0) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
      printe("%s: ERROR: Failed to init freetype.\n", __func__);
      return -1;
    }
    FT_Face face;
    if (FT_New_Face(ft, path, 0, &face)) {
      printe("%s: ERROR: Failed to load font: '%s'.\n", __func__, path);
      return -1;
    }
    FT_Set_Pixel_Sizes(face, fontw, fonth);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (Uchar c = 0; c < 128; ++c) {
      if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
        printe("%s: ERROR: Failed to load Glyph.\n", __func__);
        continue;
      }
      Uint texture;
      glGenTextures(1, &texture);
      glBindTexture(GL_TEXTURE_2D, texture);
      glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        face->glyph->bitmap.width,
        face->glyph->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        face->glyph->bitmap.buffer
      );
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      Character character = {
        texture,
        ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        (Uint)face->glyph->advance.x
      };
      characters.insert(std::pair<char, Character>(c, character));
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    size.w = fontw;
    size.h = fonth;
    return 0;
  }
};

/* Struct forward decl`s. */
struct ElementAnimationData;
struct ElementData;
struct Element;
struct Button;
struct DropDownMenu;

/* Class forward decl`s. */
class GridMap;
class FrameTimer;

class __glewApp {
 private:
  static __glewApp *_instance;
  glSsbo<ElementData, 0> _element_data_ssbo;
  glSsbo<vec2, 1> _vertices;
  glSsbo<Uint, 2> _indices;
  FrameTimer *_frame_timer;
  std::function<void()> _main_loop_lambda;
  mat4 _proj;
  /* Font data. */
  Uint _fontVAO, _fontVBO, _fontshader;
  Font _font;

  static void _destroy(void);

  void _delete_data(void);
  void _register_element(Element *e);
  void _cleanup(void);
  void _shader_init(void);
  int  _init(const char *title);
  bool _put_element_over(Element *e_under, Element *e_over);
  void _remove_element(const std::string &name);
  Element *_root(void);

 public:
  std::unordered_map<std::string, Element *> elements;
  MVector<Element *> element_vec;
  MVector<Pair<Element *, ElementAnimationData>> animation_data;
  GridMap *grid_map;
  vec2 size;
  dvec2 mouse;
  bit_flag_t<8> flag;
  GLFWwindow *win;
  Uint shader;

  static __glewApp *instance(void);

  void setup(const char *title, const vec4 &root_color, const vec2 &win_size = vec2(800.0f, 600.0f), Uint fps = 240);
  void set_font(const char *path, Uint size);
  Element *new_element(const std::string &parent, const std::string &name, const vec2 &pos, const vec2 &size, const vec4 &color, const vec4 &hi_color);
  Element *get_element(const std::string &name);
  Element *get_element_from_mouse(void);
  Button *new_button(const std::string &parent, const std::string &name, const vec2 &pos, const vec2 &size, const vec4 &color, const vec4 &hi_color);
  DropDownMenu *new_dropdown_menu(const std::string &parent, const std::string &name, const vec2 &pos, const vec2 &size, const vec4 &color, const vec4 &hi_color, int dropdown_side);

  bool is_ancestor(Element *e, Element *ancestor);
  void move_element(Element *e, const vec2 &pos);
  void resize_element(Element *e, const vec2 &size);
  void move_resize_element(Element *e, const vec2 &pos, const vec2 &size);
  void animate_element(Element *e, const vec2 &end_pos, const vec2 &end_size, Uint duration_ms);
  void render_text(std::string text, vec2 pos, float scale, vec3 color);
  void set_main_loop_lambda(const std::function<void()> &lambda);
  void draw(void);
  void run(void);
};

struct ElementAnimationData {
  Uint steps;
  vec4 start;
  vec4 end;
  vec4 step;

  ElementAnimationData(const vec2 &start_pos, const vec2 &start_size, const vec2 &end_pos, const vec2 &end_size, Uint duration_ms, Uint fps)
  : start(start_pos.x, start_pos.y, start_size.x, start_size.y), end(end_pos.x, end_pos.y, end_size.x, end_size.y) {
    steps = (duration_ms / (1000.0f / fps));
    step = ((end - start) / steps);
  }
};

struct ElementData {
  vec2 pos;
  vec2 size;
  vec4 color;
};

struct Element {
  std::string name;
  ElementData data;
  vec2 pos_alignment;
  bit_flag_t<ELEMENT_FLAG_SIZE> flag;
  Element *parent;
  MVector<Element *> children;
  vec4 color;
  vec4 hi_color;
  std::function<void()> enter_action;
  std::function<void()> leave_action;
  std::string text;
  vec3 textcolor;
  vec3 texthicolor;
  
  /* Constructor. */
  Element(const std::string &name, const vec2 &pos, const vec2 &size, const vec4 &color, const vec4 &hi_color)
  : name(name), data{pos, size, color}, color(color), hi_color(hi_color) {}

  template <typename Callback>
  void set_enter_action(Callback &&callback) {
    enter_action = std::forward<Callback>(callback);
  }

  template <typename Callback>
  void set_leave_action(Callback &&callback) {
    leave_action = std::forward<Callback>(callback);
  }

  void set_text_data(const std::string &text, const vec3 &textcolor, const vec3 &texthicolor) {
    this->text = text;
    this->textcolor = textcolor;
    this->texthicolor = texthicolor; 
  }
};

struct Button : Element {
  std::string lable;
  std::function<void()> on_click;
  std::function<void()> on_hover;

  Button(const std::string &name, const vec2 &pos, const vec2 &size, const vec4 &color, const vec4 &hi_color)
  : Element(name, pos, size, color, hi_color) {}

  template <typename Callback>
  void set_on_click(Callback &&callback) {
    on_click = std::forward<Callback>(callback);
  }
};

struct DropDownMenu : Button {
  DropDownMenu(const std::string &name, const vec2 &pos, const vec2 &size, const vec4 &color, const vec4 &hi_color, int dropdown_side)
  : Button(name, pos, size, color, hi_color) {
    flag.set<PARENT_ALWAYS_OVER_CHILDREN>();
    flag.set<DO_NOT_PREDUSE_LEAVE_EVENT_ON_CHILDREN_ENTER>();
    switch (dropdown_side) {
      case DROPDOWN_MENU_LEFT_SIDE :
        flag.set<DROPDOWN_MENU_LEFT_SIDE>();
        break;
      case DROPDOWN_MENU_RIGHT_SIDE :
        flag.set<DROPDOWN_MENU_RIGHT_SIDE>();
        break;
      case DROPDOWN_MENU_TOP_SIDE :
        flag.set<DROPDOWN_MENU_TOP_SIDE>();
        break;
      case DROPDOWN_MENU_DOWN_SIDE :
        flag.set<DROPDOWN_MENU_DOWN_SIDE>();
        break;
    }
    flag.set(dropdown_side);
    set_enter_action([this]() {
      Uint idx = 0;
      for (Element *e : children) {
        auto &[pos, size, color] = data;
        if (flag.is_set<DROPDOWN_MENU_DOWN_SIDE>()) {
          glApp->move_resize_element(e, vec2(pos.x, (pos.y + (size.y * ++idx))), size);
        }
        else if (flag.is_set<DROPDOWN_MENU_TOP_SIDE>()) {
          glApp->move_resize_element(e, vec2(pos.x, (pos.y - (size.y * ++idx))), size);
        }
        else if (flag.is_set<DROPDOWN_MENU_RIGHT_SIDE>()) {
          glApp->move_resize_element(e, vec2((pos.x + size.x), (pos.y + (size.y * idx++))), size);
        }
        else if (flag.is_set<DROPDOWN_MENU_LEFT_SIDE>()) {
          glApp->move_resize_element(e, vec2((pos.x - size.x), (pos.y + (size.y * idx++))), size);
        }
      }
    });
    set_leave_action([this](){
      for (Element *e : children) {
        glApp->move_element(e, -100.0f);
      }
    });
  }
};

struct CoordHash {
  Ulong operator()(const ivec2 &coord) const {
    Ulong hash_x = std::hash<int>()(coord.x);
    Ulong hash_y = std::hash<int>()(coord.y);
    return hash_x ^ (hash_y * 0x9e3779b9 + (hash_x << 6) + (hash_x >> 2));
  }
};

class GridMap {
 private:
  int cell_size;
  std::unordered_map<ivec2, MVector<Element *>, CoordHash> grid;

  ivec2 to_grid_pos(const ivec2 &pos) const {
    return ivec2(pos / cell_size);
  }

 public:
  GridMap(int cell_size) : cell_size(cell_size) {};

  void set(const ivec2 &pos, Element *element) {
    grid[to_grid_pos(pos)].emplace_back(element);
  }

  void set(const ivec2 &pos, const ivec2 &size, Element *element) {
    ivec2 start = to_grid_pos(pos);
    ivec2 end   = to_grid_pos(pos + size);
    for (int x = start.x; x <= end.x; ++x) {
      for (int y = start.y; y <= end.y; ++y) {
        grid[ivec2(x, y)].emplace_back(element);
      }
    }
  }

  void set(Element *e) {
    set(e->data.pos, e->data.size, e);
  }

  void remove(const ivec2 &pos, Element *e) {
    ivec2 at_pos = to_grid_pos(pos);
    auto &vector = grid.at(at_pos);
    for (Uint i = 0; i < vector.size(); ++i) {
      if (grid.at(at_pos)[i] == e) {
        vector.erase(&vector[i--]);
      }
    }
    if (vector.empty()) {
      grid.erase(at_pos);
    }
  }

  /* Remove all entries for a given element, at all grids based on its size. */
  void remove(const ivec2 &pos, const ivec2 &size, Element *e) {
    ivec2 start = to_grid_pos(pos);
    ivec2 end   = to_grid_pos(pos + size);
    for (int x = start.x; x <= end.x; ++x) {
      for (int y = start.y; y <= end.y; ++y) {
        ivec2 at_pos(x, y);
        auto &vector = grid.at(at_pos);
        for (Uint i = 0; i < vector.size(); ++i) {
          if (vector[i] == e) {
            vector.erase(&vector[i--]);
          }
        }
        if (vector.empty()) {
          grid.erase(at_pos);
        }
      }
    }
  }

  void remove(Element *e) {
    remove(e->data.pos, e->data.size, e);
  }

  Element *get(const ivec2 &pos) const {
    auto it = grid.find(to_grid_pos(pos));
    if (it == grid.end()) {
      return nullptr;
    }
    for (Uint i = 0; i < it->second.size(); ++i) {
      if (pos.x >= it->second[i]->data.pos.x && pos.x <= (it->second[i]->data.pos.x + it->second[i]->data.size.x)
      && pos.y >= it->second[i]->data.pos.y && pos.y <= (it->second[i]->data.pos.y + it->second[i]->data.size.y)) {
        return it->second[i];
      }
    }
    return nullptr;
  }

  bool contains(const ivec2 &pos) const {
    return (grid.find(to_grid_pos(pos)) != grid.end());
  }
};

class FrameTimer {
 private:
  TIME_POINT<HIGH_RES_CLOCK> _start;
  TIME_POINT<HIGH_RES_CLOCK> _end;

 public:
  Uint fps;

  void start(void) {
    _start = HIGH_RES_CLOCK::now();
  }

  void end(void) {
    _end = HIGH_RES_CLOCK::now();
    Duration<double, Milli> frame_duration = (_end - _start);
    if (frame_duration.count() >= (1000.0f / fps)) {
      return;
    }
    Duration<double, Milli> sleep_duration((1000.0f / fps) - frame_duration.count());
    std::this_thread::sleep_for(sleep_duration);
  }
};