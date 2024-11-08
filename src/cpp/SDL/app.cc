#include <cstdlib>
#include <cstring>
#include <new>
#include <thread>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "Debug.h"
#include "Mem_pool.h"
#include "Profile.h"
#include "def.h"

#include "SDL/SDL_def.hpp"
#include "SDL/app.hpp"
#include "SDL/element.hpp"
#include "SDL/event_handler.hpp"
#include "SDL/window.hpp"

SDL_Texture *texture_exceptions::blank_texture = nullptr;

/*-<< sdl_app >>-*/
app *app::_instance = nullptr;

/*-<< Constructor >>-*/
app::app(void) noexcept
    : mem_pool(100 * MB) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    logE("Failed to init sdl, SDL_ERROR: %s.", SDL_GetError());
    exit(1);
  }
  if (!TTF_Init()) {
    logE("Failed to init ttf, SLD_ERROR: %s.", SDL_GetError());
    SDL_Quit();
    exit(1);
  }
  root = mem_pool.alloc<window>();
  if (!root) {
    logI("Falied to alloc root.");
    TTF_Quit();
    SDL_Quit();
    exit(1);
  }
  root->init();
  _flags.set(APP_RUNNING);
  EVENT_HANDLER->event_action(SDL_EVENT_WINDOW_RESIZED, [this](SDL_Event ev) {
    root->w = (float)ev.window.data1;
    root->h = (float)ev.window.data2;
    for (const auto &elem : root->elements) {
      if (elem->flags.is_set<element::ALIGN_X>()) {
        elem->rect.x = (root->w + elem->align_rect.x);
      }
      if (elem->flags.is_set<element::ALIGN_Y>()) {
        elem->rect.y = (root->h + elem->align_rect.y);
      }
      if (elem->flags.is_set<element::ALIGN_WIDTH>()) {
        elem->rect.w = (root->w - elem->rect.x);
      }
      if (elem->flags.is_set<element::ALIGN_HEIGHT>()) {
        elem->rect.h = (root->h - elem->rect.y);
      }
    }
  });
  EVENT_HANDLER->event_action(SDL_EVENT_QUIT, [this](SDL_Event ev) {
    quit();
  });
}

/*-<< Destructor >>-*/
void app::_destroy(void) noexcept {
  delete _instance;
}

/* Dealloc all obj saved during runtime, this way all
 * heap mem is handled centraly and automaticly. */
void app::_cleanup(void) noexcept {
  if (texture_exceptions::blank_texture) {
    SDL_DestroyTexture(texture_exceptions::blank_texture);
  }
  for (const auto &t : _alloced_textures) {
    SDL_DestroyTexture(t);
    /* Sanity check so that i know that no textures got destroyd before this. */
    if (strcmp(SDL_GetError(), "Invalid texture") == 0) {
      logE("SDL_DestroyTexture recieved a invalid alloced texture.");
    }
  }
  for (const auto &f : _alloced_fonts) {
    TTF_CloseFont(f);
  }
  root->cleanup();
  TTF_Quit();
  SDL_Quit();
}

void app::_perform_amimation(void) noexcept {
  for (Uint i = 0; i < _anim_data.size(); ++i) {
    auto &[steps, end_x, end_y, end_w, end_h, step_x, step_y, step_w, step_h] = _anim_data[i].second;
    if (steps <= 0.00) {
      _anim_data[i].first->flags.unset<element::IN_ANIMATION>();
      _anim_data[i].first->rect.x = end_x;
      _anim_data[i].first->rect.y = end_y;
      _anim_data[i].first->rect.w = end_w;
      _anim_data[i].first->rect.h = end_h;
      _anim_data.erase_at(i);
      i -= 1;
    }
    else {
      PROFILE_FUNCTION;
      steps -= 1.00;
      _anim_data[i].first->rect.x += step_x;
      if ((step_x < 0.00 && _anim_data[i].first->rect.x < end_x) ||
          (step_x > 0.00 && _anim_data[i].first->rect.x > end_x)) {
        _anim_data[i].first->rect.x = end_x;
      }
      _anim_data[i].first->rect.y += step_y;
      if ((step_y < 0.00 && _anim_data[i].first->rect.y < end_y) ||
          (step_y > 0.00 && _anim_data[i].first->rect.y > end_y)) {
        _anim_data[i].first->rect.y = end_y;
      }
      _anim_data[i].first->rect.w += step_w;
      if ((step_w < 0.00 && _anim_data[i].first->rect.w < end_w) ||
          (step_w > 0.00 && _anim_data[i].first->rect.w > end_w)) {
        _anim_data[i].first->rect.w = end_w;
      }
      _anim_data[i].first->rect.h += step_h;
      if ((step_h < 0.00 && _anim_data[i].first->rect.h < end_h) ||
          (step_h > 0.00 && _anim_data[i].first->rect.h > end_h)) {
        _anim_data[i].first->rect.h = end_h;
      }
    }
  }
}

void app::_frame_start(void) noexcept {
  _frame_start_time = high_resolution_clock::now();
}

void app::_frame_end(void) noexcept {
  time_point<high_resolution_clock> end            = high_resolution_clock::now();
  duration<double, milli>           dur            = end - _frame_start_time;
  auto                              delay_duration = duration<double, milli>(root->ms_frame_delay - dur.count());
  __this_thread::sleep_for(delay_duration);
}

/* This is the only way to access the app. */
app *app::instance(void) noexcept {
  if (_instance == nullptr) {
    _instance = new (std::nothrow) app();
    if (_instance == nullptr) {
      logE("Mem alloc failed, 'new(std::nothrow)' returned a nullptr.");
      exit(1);
    }
    atexit(_destroy);
  }
  return _instance;
}

/* This is the rendering loop. */
int app::run(void) noexcept {
  root->set_position(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
  while (_flags.is_set<APP_RUNNING>()) {
    PROFILE_FUNCTION;
    _frame_start();
    SDL_SetRenderDrawColor(root->ren, 0, 0, 0, 255);
    SDL_RenderClear(root->ren);
    _perform_amimation();
    for (const auto &elem : root->elements) {
      elem->draw();
    }
    if (_main_loop) {
      _main_loop();
    }
    EVENT_HANDLER->handle_event();
    KEY_HANDLER->handleKeyEvent();
    SDL_RenderPresent(root->ren);
    _frame_end();
  }
  _cleanup();
  return 0;
}

/* Set the main logic loop for the app. */
void app::set_main_loop(const func<void(void)> &loop) noexcept {
  _main_loop = loop;
}

/* Tell the app to stop running. */
void app::quit(void) noexcept {
  _flags.unset<APP_RUNNING>();
}

/* Retrieve a font file at 'size', do NOT free the font for any reason the alloced
 * memory is fully managed by 'sdl_app' internaly. */
TTF_Font *app::retrieve_new_font(const int size, const char *file) noexcept {
  TTF_Font *font = TTF_OpenFont(file, size);
  if (font == nullptr) {
    logE("Failed to open font file '%s'.", file);
    return nullptr;
  }
  _alloced_fonts.push_back(font);
  return font;
}

SDL_Texture *app::make_text_texture(TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg) noexcept {
  /* If the 'text' string is empty we do not return a nullptr,
   * instead we return an texture exception.  A blank 1x1 texture. */
  if (text[0] == '\0') {
    if (!texture_exceptions::blank_texture) {
      texture_exceptions::blank_texture =
        SDL_CreateTexture(root->ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1, 1);
      if (!texture_exceptions::blank_texture) {
        logE("Failed to create 'sdl_texture_exceptions::blank_texture'.");
      }
    }
    return texture_exceptions::blank_texture;
  }
  SDL_Surface *surface = TTF_RenderUTF8_Shaded(font, text, fg, bg);
  if (surface == nullptr) {
    logE("Failed to make surface.");
    return nullptr;
  }
  SDL_Texture *texture = SDL_CreateTextureFromSurface(root->ren, surface);
  if (texture == nullptr) {
    logE("Failed to make texture.");
    return nullptr;
  }
  SDL_DestroySurface(surface);
  if (_alloced_textures.size() > 100) {
    for (int i = 0; i < 20; i++) {
      SDL_DestroyTexture(_alloced_textures[i]);
    }
    _alloced_textures.reorder_from(20);
  }
  _alloced_textures.push_back(texture);
  return texture;
}

SDL_Texture *app::make_destroy_text_texture(SDL_Texture *old_texture, TTF_Font *font, const char *text, SDL_Color fg,
                                            SDL_Color bg) noexcept {
  if (old_texture) {
    for (unsigned int i = 0; i < _alloced_textures.size(); i++) {
      if (_alloced_textures[i] == old_texture) {
        SDL_DestroyTexture(_alloced_textures[i]);
        _alloced_textures.erase_at(i);
        break;
      }
    }
  }
  return make_text_texture(font, text, fg, bg);
}

void app::render_text(float x, float y, SDL_Texture *texture) noexcept {
  if (texture == nullptr) {
    logE("texture is nullptr.");
    return;
  }
  if (texture == texture_exceptions::blank_texture) {
    return;
  }
  float w;
  float h;
  SDL_GetTextureSize(texture, &w, &h);
  SDL_FRect rect = {x, y, w, h};
  SDL_RenderTexture(root->ren, texture, nullptr, &rect);
}

float app::width(void) const noexcept {
  return root->w;
}

float app::height(void) const noexcept {
  return root->h;
}

void app::exit_clean(const int status) noexcept {
  _cleanup();
  exit(status);
}

void app::animate_element(element *element, element_animation_data data) noexcept {
  if (!element->flags.is_set<element::IN_ANIMATION>()) {
    element->flags.set<element::IN_ANIMATION>();
    _anim_data.push_back({element, data});
  }
}
