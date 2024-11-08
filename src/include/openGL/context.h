#pragma once

#include <GL/glew.h>
#include <SDL2/SDL.h>

SDL_GLContext openGL_init_SDL_GLContext(SDL_Window *const win, SDL_Renderer *const ren, bool experimental);