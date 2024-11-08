#pragma once

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <functional>
#include <utility>

#include "../Attributes.h"
#include "../Flag.h"
#include "../Init_list.h"
#include "../Mem_pool.h"
#include "../Vector.h"
#include "../def.h"

#include "SDL_def.hpp"
#include "element.hpp"
#include "window.hpp"

struct texture_exceptions {
  static SDL_Texture *blank_texture;
};

struct element_animation_data {
  float steps;
  float end_x;
  float end_y;
  float end_w;
  float end_h;
  float step_x;
  float step_y;
  float step_w;
  float step_h;
};

class app {
  static app            *_instance;
  std::function<void()>  _main_loop = nullptr;
  bit_flag_t<8>          _flags;
  MVector<SDL_Texture *> _alloced_textures;
  MVector<TTF_Font *>    _alloced_fonts;

  MVector<std::pair<element *, element_animation_data>> _anim_data;

  DEL_CM_CONSTRUCTORS(app);

  app(void) noexcept;
  static void _destroy(void) noexcept;
  void        _cleanup(void) noexcept;
  void        _perform_amimation(void) noexcept;
  /* These are used to calculate correct frame dalay based on time to render frame. */
  TIME_POINT<HIGH_RES_CLOCK> _frame_start_time;
  void                       _frame_start(void) noexcept;
  void                       _frame_end(void) noexcept;

 public:
  mem_pool_t<8> mem_pool;
  window       *root;

  static app *__warn_unused  instance(void) noexcept;
  int                        run(void) noexcept;
  void                       set_main_loop(const std::function<void()> &loop) noexcept;
  void                       quit(void) noexcept;
  TTF_Font *__warn_unused    retrieve_new_font(int size, const char *file) noexcept __no_null(3);
  SDL_Texture *__warn_unused make_text_texture(TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg) noexcept
    __no_null(2, 3);
  SDL_Texture *__warn_unused make_destroy_text_texture(SDL_Texture *old_texture, TTF_Font *font, const char *text,
                                                       SDL_Color fg, SDL_Color bg) noexcept __no_null(3, 4);
  void                       render_text(float x, float y, SDL_Texture *texture) noexcept;
  float __warn_unused        width(void) const noexcept;
  float __warn_unused        height(void) const noexcept;
  void __no_return           exit_clean(int status) noexcept;
  void                       animate_element(element *element, element_animation_data data) noexcept;
};

/* Flags for 'sdl_app'. */
#define APP_RUNNING 1
