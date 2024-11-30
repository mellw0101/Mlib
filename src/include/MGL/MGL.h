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

#include <stb/stb_truetype.h>

#define GRAY_VEC4(gray_scale)  vec4(vec3(gray_scale), 1.0f)

#define WHITE_VEC4 vec4(1.0f)
#define BLACK_VEC4 vec4(vec3(0.0f), 1.0f)
#define RED_VEC4   vec4(1.0f, 0.0f, 0.0f, 1.0f)
#define GREEN_VEC4 vec4(0.0f, 1.0f, 0.0f, 1.0f)
#define BLUE_VEC4  vec4(0.0f, 0.0f, 1.0f, 1.0f)

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
};
enum /* '__glewApp' Flag`s. */ {
  GL_APP_RUNNING,
  GL_HAS_MAIN_LOOP_LAMBDA
};

struct Glyph {
  vec2 pos;       /* Position of the glyph atlas. */
  vec2 size;      /* Size of the glyph. */
  vec2 offset;    /* Offset for baseline alignment. */
  float advance;  /* Horizontal advance. */
};

struct Font {
  Uint texture;
  std::unordered_map<char, Glyph> glyphs;
  ivec2 atlas_size;

  Font(const char *path, int font_size) {
    /* Read file in a thread safe manner. */
    Uchar *font_data = (Uchar *)read_file_with_lock(path);
    if (!font_data) {
      printe("Failed to open file: '%s'.\n", path);
      exit(1);
    }
    stbtt_fontinfo font_info;
    if (!stbtt_InitFont(&font_info, font_data, 0)) {
      printe("Failed to init font.  File: '%s'.\n", path);
      free(font_data);
      exit(1);
    }
    /* Scale factor for the font. */
    float scale = stbtt_ScaleForPixelHeight(&font_info, font_size);
    /* Generate glyphs and atlas. */
    atlas_size = ivec2(512);
    Uchar *atlas_data = (Uchar *)calloc(atlas_size.w * atlas_size.h, 1);
    ivec2 offset(0);
    int row_height = 0;
    for (char c = 32; c < 127; ++c) /* Basic ASCI range. */ {
      int width, height, xoff, yoff;
      Uchar *bitmap = stbtt_GetCodepointBitmap(&font_info, 0, scale, c, &width, &height, &xoff, &yoff);
      if (offset.x + width >= atlas_size.w) {
        offset.x = 0;
        offset.y += row_height;
        row_height = 0;
      }
      row_height = std::max(row_height, height);
      if (offset.y + height > atlas_size.y) {
        printe("Atlas size to small.");
        free(font_data);
        free(atlas_data);
        exit(1);
      }
      /* Copy glyph bitmap into atlas. */
      for (int y = 0; y < height; ++y) {
        memcpy((atlas_data + (offset.y + y) * atlas_size.w + offset.x), (bitmap + y * width), width);
      }
      /* Save Glyph metadata. */
      int advance_width, left_bearing;
      stbtt_GetCodepointHMetrics(&font_info, c, &advance_width, &left_bearing);
      glyphs[c] = {
        vec2(((float)offset.x / atlas_size.w), ((float)offset.y / atlas_size.h)),
        vec2(((float)width / atlas_size.w), ((float)height / atlas_size.h)),
        vec2((float)xoff, (float)yoff),
        (float)(scale * advance_width)
      };
      offset.x += width;
      stbtt_FreeBitmap(bitmap, NULL);
    }
    /* Generate OpenGL texture. */
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, atlas_size.w, atlas_size.h, 0, GL_R8, GL_UNSIGNED_BYTE, atlas_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    free(font_data);
    free(atlas_data);
    glActiveTexture(GL_TEXTURE0);
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
  void render_font_slow(const Font *font, const std::string &text, vec2 pos, float scale);
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