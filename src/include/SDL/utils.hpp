#pragma once

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>

#include "../Vector.h"
#include "../def.h"

#include "app.hpp"

SDL_Texture *create_sdl_texture_from_bitfield(SDL_Renderer *ren, MVector<MVector<int>> data);
SDL_Texture *create_sdl_texture_from_bitfield(SDL_Renderer *ren, int *pix_data[], Uint size);
void         set_ren_color(SDL_Renderer *ren, SDL_Color *c) noexcept;
