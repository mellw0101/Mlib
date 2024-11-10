#pragma once

#include "../Pair.h"
#include "../Vector.h"

#include <GL/glew.h>
#include <SDL2/SDL.h>

SDL_GLContext openGL_init_SDL_GLContext(SDL_Window *const win, bool experimental);
/* Initialize SDL and set all GL attribute pair`s. */
void openGL_init_SDL_with_GL_attr(const MVector<Pair<SDL_GLattr, int>> &gl_attributes);;