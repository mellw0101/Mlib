#include "SDL/event_handler.hpp"
#include <SDL3/SDL_events.h>
#include <cstdlib>
#include <new>
#include "Debug.h"

event_handler *event_handler::_instance = nullptr;

event_handler *event_handler::instance(void) noexcept {
  if (!_instance ) {
    _instance = new (std::nothrow) event_handler();
    if (!_instance) {
      logE("Failed to create SXL::event_handler");
      exit(1);
    }
    atexit([] {
      delete _instance;
    });
  }
  return _instance;
}

void event_handler::handle_event(void) noexcept {
  while (SDL_PollEvent(&_ev)) {
    const Uint type = event_to_index(_ev.type);
    if (type == (Uint)-1 || _map[type].empty()) {
      continue;
    }
    for (const auto &func : _map[type]) {
      func(_ev);
    }
  }
}
