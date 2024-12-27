#include "../../include/MGL/MGL.h"
#include "def.h"
#include "openGL/shader.h"

__glewApp *__glewApp::_instance = nullptr;

void __glewApp::_destroy(void) {
  _instance->_delete_data();
  delete _instance;
}

void __glewApp::_delete_data(void) {
  delete grid_map;
  delete _frame_timer;
  for (auto &it : element_vec) {
    delete it;
  }
  elements.clear();
  element_vec.clear();
  glDeleteProgram(shader);
}

void __glewApp::_register_element(Element *e) {
  elements[e->name] = e;
}

void __glewApp::_cleanup(void) {
  glfwDestroyWindow(win);
  glfwTerminate();
}

void __glewApp::_shader_init(void) {
  shader = openGL_create_shader_program_raw({
    { STRLITERAL(\
        #version 450 core \n
        struct ElementData {
          vec2 pos;
          vec2 size;
          vec4 color;
        };
        layout(std430, binding = 0) buffer ElementDataBuffer {
          ElementData data[];
        };
        layout(std430, binding = 1) buffer VertexBuffer {
          vec2 vertices[];
        };
        layout(std430, binding = 2) buffer IndexBuffer {
          uint indices[];
        };
        /* Output. */
        out vec4 vertexColor;
        /* Uniforms. */
        uniform mat4 projection;
        /* Main vertex shader exec. */
        void main() {
          ElementData elem = data[gl_InstanceID];
          vec2 scaledPos   = (vertices[indices[gl_VertexID]] * elem.size);
          vec4 worldPos    = (projection * vec4(scaledPos + elem.pos, 0.0f, 1.0f));
          gl_Position      = worldPos;
          vertexColor      = elem.color;
        }
      ),
      GL_VERTEX_SHADER },
    { STRLITERAL(\
        #version 450 core \n
        /* Output. */
        out vec4 FragColor;
        /* Input. */
        in vec4 vertexColor;
        /* Uniforms. */
        /* Main shader exec. */
        void main() {
          FragColor = vertexColor;
        }
      ),
      GL_FRAGMENT_SHADER }
  });
  _fontshader = openGL_create_shader_program_raw({
    { STRLITERAL( \
        #version 330 core\n
        layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
        out vec2 TexCoords;

        uniform mat4 projection;

        void main()
        {
          gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
          TexCoords = vertex.zw;
        }
      ),
      GL_VERTEX_SHADER },
    { STRLITERAL(\
        #version 330 core\n
        in vec2 TexCoords;
        out vec4 color;

        uniform sampler2D text;
        uniform vec3 textColor;

        void main()
        {    
          vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
          color = vec4(textColor, 1.0) * sampled;
        }   
      ),
      GL_FRAGMENT_SHADER }
  });
  glGenVertexArrays(1, &_fontVAO);
	glGenBuffers(1, &_fontVBO);
	glBindVertexArray(_fontVAO);
	glBindBuffer(GL_ARRAY_BUFFER, _fontVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

int __glewApp::_init(const char *title) {
  if (!glfwInit()) {
    printe("Failed to init glfw.\n");
    return 1;
  }
  win = glfwCreateWindow(size.w, size.h, (!title ? "placeholder" : title), NULL, NULL);
  if (!win) {
    printe("Failed to create glfw window.\n");
    glfwTerminate();
    return 1;
  }
  glfwMakeContextCurrent(win);
  if (glewInit() != GLEW_OK) {
    printe("Failed to init glew.\n");
    glfwDestroyWindow(glApp->win);
    glfwTerminate();
    return 1;
  }
  _shader_init();
  glViewport(0, 0, size.w, size.h);
  _proj = ortho_projection(0.0f, size.w, 0.0f, size.h);
  glUseProgram(shader);
  glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &_proj[0][0]);
  glUseProgram(_fontshader);
  glUniformMatrix4fv(glGetUniformLocation(_fontshader, "projection"), 1, GL_FALSE, &_proj[0][0]);
  glfwSetWindowSizeCallback(win, [](GLFWwindow *win, int w, int h) {
    glApp->size = vec2(w, h);
    glViewport(0, 0, glApp->size.w, glApp->size.h);
    glApp->_proj = ortho_projection(0.0f, glApp->size.w, 0.0f, glApp->size.h);
    glUseProgram(glApp->shader);
    glUniformMatrix4fv(glGetUniformLocation(glApp->shader, "projection"), 1, GL_FALSE, &glApp->_proj[0][0]);
    glUseProgram(glApp->_fontshader);
    glUniformMatrix4fv(glGetUniformLocation(glApp->_fontshader, "projection"), 1, GL_FALSE, &glApp->_proj[0][0]);
    glApp->_root()->data.size = vec2(glApp->size.w, glApp->size.h);
    for (const auto &[name, e] : glApp->elements) {
      if (e->flag.is_set<ALIGN_SIZE_HEIGHT>()) {
        glApp->resize_element(e, vec2(e->data.size.w, (glApp->size.h - e->data.pos.y)));
      }
      if (e->flag.is_set<ALIGN_POS_WIDTH>()) {
        glApp->move_element(e, vec2((glApp->size.w - e->pos_alignment.x - e->data.size.x), e->data.pos.y));
      }
    }
  });
  glfwSetKeyCallback(win, [](GLFWwindow *win, int key, int scancode, int action, int mods) {
    switch (key) {
    case GLFW_KEY_ESCAPE:
      glApp->flag.unset<GL_APP_RUNNING>();
      break;
    }
  });
  glfwSetMouseButtonCallback(win, [](GLFWwindow *win, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
      if (button == GLFW_MOUSE_BUTTON_1) {
        Element *e = glApp->get_element_from_mouse();
        if (e) {
          if (e->flag.is_set<IS_BUTTON>()) {
            Button *b = (Button *)e;
            if (b->on_click) {
              b->on_click();
            }
          }
          printf("element: %s pressed, x: %.0lf, y: %.0lf.\n", e->name.c_str(), glApp->mouse.x, glApp->mouse.y);
        }
      }
    }
  });
  glfwSetCursorPosCallback(win, [](GLFWwindow *win, double posx, double posy) {
    glApp->mouse = dvec2(posx, posy);
  });
  glfwSetCursorEnterCallback(win, [](GLFWwindow *win, int entered) {
    if (!entered) {
      glApp->mouse = vec2(-30.0f);
    }
  });
  return 0;
}

bool __glewApp::_put_element_over(Element *e_under, Element *e_over) {
  /* Return early if params are invalid or the same element. */
  if (!e_under || !e_over || e_under == e_over) {
    return false;
  }
  Uint under_idx = -1, over_idx = -1;
  for (Uint i = 0; i < element_vec.size(); ++i) {
    if (element_vec[i] == e_under) {
      /* Return directly if we find the under idx first. */
      if (over_idx == -1) {
        return false;
      }
      under_idx = i;
    }
    if (element_vec[i] == e_over) {
      over_idx = i;
    }
    /* Break early when both elements are found. */
    if (under_idx != -1 && over_idx != -1) {
      break;
    }
  }
  element_vec.erase_at(over_idx);
  element_vec.insert(under_idx, e_over);
  return true;
}

void __glewApp::_remove_element(const std::string &name) {
  Element *e = get_element(name);
  if (name == "root" || !e) {
    return;
  }
  /* Recursive deletion of children */
  for (Element *child : e->children) {
    _remove_element(child->name);
  }
  /* Remove from grid map and element data */
  grid_map->remove(e);
  elements.erase(name);
  element_vec.erase(e);
  /* Remove related animation data */
  for (Uint i = 0; i < animation_data.size(); ++i) {
    if (animation_data[i].first == e) {
      animation_data.erase_at(i--);
    }
  }
  /* Delete the element itself */
  delete e;
}

Element *__glewApp::_root(void) {
  return get_element("root");
}

__glewApp *__glewApp::instance(void) {
  if (!_instance) {
    _instance = new __glewApp();
    _instance->grid_map = new GridMap(20);
    _instance->_frame_timer = new FrameTimer();
    atexit(_destroy);
  }
  return _instance;
}

void __glewApp::setup(const char *title, const vec4 &root_color, const vec2 &win_size, Uint fps) {
  size = win_size;
  mouse = dvec2(-30.0, -30.0);
  if (_init(title) != 0) {
    exit(1);
  }
  glUseProgram(shader);
  _element_data_ssbo.init(MVector<ElementData>(10));
  _vertices.init(vertices);
  _indices.init(indices);
  flag.set<GL_APP_RUNNING>();
  new_element("", "root", 0.0f, win_size, root_color, root_color);
  _frame_timer->fps = fps;
}

void __glewApp::set_font(const char *path, Uint size) {
  glUseProgram(_fontshader);
  _font.load_font(path, size);
  flag.set<GLAPP_HAS_ACTIVE_FONT>();
}

Element *__glewApp::new_element(const std::string &parent, const std::string &name, const vec2 &pos, const vec2 &size, const vec4 &color, const vec4 &hi_color) {
  Element *p_elem = get_element(parent);
  if (name != "root" && !p_elem) {
    return nullptr;
  }
  Element *elem = new Element(name, pos, size, color, hi_color);
  _register_element(elem);
  element_vec.push_back(elem);
  if (name != "root") {
    grid_map->set(elem);
    elem->parent = p_elem;
    elem->parent->children.push_back(elem);
  }
  else {
    elem->parent = nullptr;
  }
  return elem;
}

Button *__glewApp::new_button(const std::string &parent, const std::string &name, const vec2 &pos, const vec2 &size, const vec4 &color, const vec4 &hi_color) {
  Element *p_elem = get_element(parent);
  if (name != "root" && !p_elem) {
    return nullptr;
  }
  Button *button = new Button(name, pos, size, color, hi_color);
  button->flag.set<IS_BUTTON>();
  _register_element(button);
  element_vec.push_back(button);
  if (name != "root") {
    grid_map->set(button);
    button->parent = p_elem;
    button->parent->children.push_back(button);
  }
  return button;
}

DropDownMenu *__glewApp::new_dropdown_menu(const std::string &parent, const std::string &name, const vec2 &pos, const vec2 &size, const vec4 &color, const vec4 &hi_color, int dropdown_side) {
  Element *p_elem = get_element(parent);
  if (name != "root" && !p_elem) {
    return nullptr;
  }
  DropDownMenu *button = new DropDownMenu(name, pos, size, color, hi_color, dropdown_side);
  button->flag.set<IS_BUTTON>();
  _register_element(button);
  element_vec.push_back(button);
  if (name != "root") {
    grid_map->set(button);
    button->parent = p_elem;
    button->parent->children.push_back(button);
  }
  return button;
}

Element *__glewApp::get_element(const std::string &name) {
  return elements.count(name) ? elements[name] : nullptr;
}

Element *__glewApp::get_element_from_mouse(void) {
  ivec2 pos(mouse);
  if (grid_map->contains(pos)) {
    return grid_map->get(pos);
  }
  return nullptr;
}

bool __glewApp::is_ancestor(Element *e, Element *ancestor) {
  Element *element = e;
  while (element) {
    if (element == ancestor) {
      return true;
    }
    element = element->parent;
  }
  return false;
}

void __glewApp::move_element(Element *e, const vec2 &pos) {
  grid_map->remove(e);
  e->data.pos = pos;
  grid_map->set(e);
}

void __glewApp::resize_element(Element *e, const vec2 &size) {
  grid_map->remove(e);
  e->data.size = size;
  grid_map->set(e);
}

void __glewApp::move_resize_element(Element *e, const vec2 &pos, const vec2 &size) {
  grid_map->remove(e);
  e->data.pos = pos;
  e->data.size = size;
  grid_map->set(e);
}

void __glewApp::animate_element(Element *e, const vec2 &end_pos, const vec2 &end_size, Uint duration_ms) {
  if (!e->flag.is_set<IN_ANIMATION>()) {
    e->flag.set<IN_ANIMATION>();
    animation_data.push_back({e, {e->data.pos, e->data.size, end_pos, end_size, duration_ms, _frame_timer->fps}});
  }
}

void __glewApp::render_text(std::string text, vec2 pos, float scale, vec3 color) {
  if (flag.is_set<GLAPP_HAS_ACTIVE_FONT>()) {
    /* Activate the font shader. */
    glUseProgram(_fontshader);
    glUniform3f(glGetUniformLocation(_fontshader, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(_fontVAO);
    /* Iterate through all characters. */
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); ++c) {
      Character ch = _font.characters[*c];
      /* Define parameters for glyph. */
      float xpos = (pos.x + ch.Bearing.x * scale);
      float ypos = (pos.y + (ch.Size.y - ch.Bearing.y) * scale);
      float w = (ch.Size.x * scale);
      float h = (ch.Size.y * scale);
      /* Update VBO for each character. */
      float vertices[6][4] = {
        /* Vertices data. */
        {  xpos,      (ypos - h),  0.0f, 0.0f },
        {  xpos,       ypos,       0.0f, 1.0f },
        { (xpos + w),  ypos,       1.0f, 1.0f },
        /* Texture data. */
        {  xpos,      (ypos - h),  0.0f, 0.0f },
        { (xpos + w),  ypos,       1.0f, 1.0f },
        { (xpos + w), (ypos - h),  1.0f, 0.0f }  
      };
      /* render glyph texture over quad */
      glBindTexture(GL_TEXTURE_2D, ch.textureID);
      /* Update content of VBO memory. */
      glBindBuffer(GL_ARRAY_BUFFER, _fontVBO);
      glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      /* Render quad. */
      glDrawArrays(GL_TRIANGLES, 0, 6);
      /* Now advance cursors for next glyph (note that advance is number of 1/64 pixels). */
      pos.x += ((ch.Advance >> 6) * scale); /* Bitshift by 6 to get value in pixels (2^6 = 64) */
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
}

void __glewApp::set_main_loop_lambda(const std::function<void()> &lambda) {
  if (!lambda) {
    flag.unset<GL_HAS_MAIN_LOOP_LAMBDA>();
    return;
  }
  flag.set<GL_HAS_MAIN_LOOP_LAMBDA>();
  _main_loop_lambda = lambda;
}

void __glewApp::draw(void) {
  glUseProgram(shader);
  if (_element_data_ssbo.size() <= element_vec.size()) {
    _element_data_ssbo.resize(element_vec.size() * 2);
  }
  for (Uint i = 0; i < animation_data.size(); ++i) {
    auto &[e, data] = animation_data[i];
    if (data.steps > 0) {
      static constexpr auto update_step = [](float &current, float end, float step) {
        current = (step > 0.0f)
          ? (current < end) ? (current + step) : end
          : (current > end) ? (current + step) : end;
      };
      grid_map->remove(e);
      update_step(e->data.pos.x, data.end.x, data.step.x);
      update_step(e->data.pos.y, data.end.y, data.step.y);
      update_step(e->data.size.x, data.end.z, data.step.z);
      update_step(e->data.size.y, data.end.w, data.step.w);
      grid_map->set(e);
      --data.steps;
    }
    else {
      /* Set the absolute position of the element to the end pos. */
      grid_map->remove(e);
      e->data.pos = vec2(data.end.x, data.end.y);
      e->data.size = vec2(data.end.z, data.end.w);
      grid_map->set(e);
      e->flag.unset<IN_ANIMATION>();
      animation_data.erase_at(i--);
    }
  }
  _element_data_ssbo.perform_write_action_on_buffer_range(0, elements.size(), [&](ElementData *data, Uint count) {
    for (Uint i = 0; i < element_vec.size(); ++i) {
      if (element_vec[i]->flag.is_set<PARENT_ALWAYS_OVER_CHILDREN>() && !element_vec[i]->children.empty()) {
        if (_put_element_over(element_vec[i]->children.back(), element_vec[i])) {
          --i;
          continue;
        }
      }
      if (element_vec[i]->flag.is_set<DELETE_ELEMENT>()) {
        _remove_element(element_vec[i--]->name);
        continue;
      }
      Element *mouse_element = get_element_from_mouse();
      if (mouse_element == element_vec[i]) {
        if (element_vec[i]->enter_action) {
          element_vec[i]->enter_action();
        }
        element_vec[i]->data.color = element_vec[i]->hi_color;
        element_vec[i]->flag.set<ELEMENT_MOUSE_INSIDE>();
      }
      else {
        if (element_vec[i]->leave_action) {
          /* If 'DO_NOT_PREDUSE_LEAVE_EVENT_ON_CHILDREN_ENTER' flag is set.  Only run 'leave_action' when mouse is not on a child. */
          if (element_vec[i]->flag.is_set<DO_NOT_PREDUSE_LEAVE_EVENT_ON_CHILDREN_ENTER>()) {
            if (!is_ancestor(mouse_element, element_vec[i])) {
              element_vec[i]->leave_action();
            }
          }
          else {
            element_vec[i]->leave_action();
          }
        }
        element_vec[i]->data.color = element_vec[i]->color;
        element_vec[i]->flag.unset<ELEMENT_MOUSE_INSIDE>();
      }
      data[i] = element_vec[i]->data;
    }
  });
  glDrawArraysInstanced(GL_TRIANGLES, 0, INDICES_SIZE, element_vec.size());
  /* Draw element text. */
  for (Uint i = 0; i < element_vec.size(); ++i) {
    if (!element_vec[i]->text.empty()) {
      if (element_vec[i]->flag.is_set<ELEMENT_MOUSE_INSIDE>()) {
        render_text(element_vec[i]->text, (element_vec[i]->data.pos + _font.size), 1.0f, element_vec[i]->texthicolor);
      }
      else {
        render_text(element_vec[i]->text, (element_vec[i]->data.pos + _font.size), 1.0f, element_vec[i]->textcolor);
      }
    }
  }
}

void __glewApp::run(void) {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  while (!glfwWindowShouldClose(win) && flag.is_set<GL_APP_RUNNING>()) {
    _frame_timer->start();
    glClear(GL_COLOR_BUFFER_BIT);
    draw();
    if (flag.is_set<GL_HAS_MAIN_LOOP_LAMBDA>()) {
      _main_loop_lambda();
    }
    glfwSwapBuffers(win);
    glfwPollEvents();
    _frame_timer->end();
  }
  _cleanup();
}