#include "../../include/openGL/context.h"

SDL_GLContext openGL_init_SDL_GLContext(SDL_Window *const win, bool experimental) {
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

void openGL_init_SDL_with_GL_attr(const MVector<Pair<SDL_GLattr, int>> &gl_attributes) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
    exit(1);
  }
  /* Set all attribute pairs. */
  for (const auto &attr : gl_attributes) {
    /* If an attribute failes to be set, print the error, then terminate imedietly. */
    if (SDL_GL_SetAttribute(attr.first, attr.second) != 0) {
      fprintf(stderr, "Failed to set attr: %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
    }
  }
}

