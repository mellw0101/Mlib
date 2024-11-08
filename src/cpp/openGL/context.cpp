#include "../../include/openGL/context.h"

SDL_GLContext openGL_init_SDL_GLContext(SDL_Window *const win, SDL_Renderer *const ren, bool experimental) {
  SDL_GLContext context = SDL_GL_CreateContext(win);
  if (!context) {
    fprintf(stderr, "Failed to create SDL_GLContext.\n");
    return nullptr;
  }
  glewExperimental = experimental;
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to init glew.\n");
    return nullptr;
  }
  return context;
}
