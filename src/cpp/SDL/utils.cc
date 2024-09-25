#include "SDL/utils.hpp"
#include "Debug.h"
#include "SDL/SDL_def.hpp"
#include "Vector.h"
#include "def.h"
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <cstdlib>

SDL_Texture *
create_sdl_texture_from_bitfield(SDL_Renderer *ren, MVector<MVector<int>> data)
{
    int  width  = (int)data.size();
    int  height = (int)data.size();
    int *pixels = APP->mem_pool.alloc<int>(((width + 1) * (height + 1)));
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int           brightness      = data[x][y];
            unsigned char grayscale_value = (brightness * 255) / 100;
            unsigned int  color =
                (255 << 24) | (grayscale_value << 16) | (grayscale_value << 8) | grayscale_value;
            pixels[y * width + x] = (int)color;
        }
    }
    /* The pitch is the number of bytes per row, which is width * 4 (for 32-bit RGBA) */
    int pitch = width * 4;
    /* Create a surface from the pixel data using the correct SDL3 function */
    SDL_Surface *surface =
        SDL_CreateSurfaceFrom(width, height, SDL_PIXELFORMAT_RGBA32, pixels, pitch);
    if (surface == nullptr)
    {
        /* Handle error in case SDL_CreateSurfaceFrom fails */
        logE("Failed to create surface: %s", SDL_GetError());
        free(pixels);
        return nullptr;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surface);
    if (texture == nullptr)
    {
        logE("Failed to create texture from bitfield surface, SDL_ERROR: %s.", SDL_GetError());
    }
    SDL_DestroySurface(surface);
    free(pixels);
    return texture;
}

SDL_Texture *
create_sdl_texture_from_bitfield(SDL_Renderer *ren, int *pix_data[], Uint size)
{
    int width  = size;
    int height = size;
    /* Allocate memory for pixel data */
    int *pixels = AMALLOC_ARRAY(pixels, (width * height));
    if (pixels == nullptr)
    {
        logE("Memory allocation for pixels failed.");
        return nullptr;
    }
    /* Populate pixel array with grayscale colors */
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int brightness = pix_data[x][y];
            if (brightness < 0) brightness = 0;
            if (brightness > 100) brightness = 100; /* Ensure brightness is clamped between 0-100 */
            
            unsigned char grayscale_value = (brightness * 255) / 100;
            unsigned int  color = 
                (255 << 24) | (grayscale_value << 16) | (grayscale_value << 8) | grayscale_value;
            pixels[y * width + x] = (int)color;
        }
    }

    /* The pitch is the number of bytes per row: width * 4 (for 32-bit RGBA) */
    int pitch = width * 4;

    /* Create a surface from the pixel data */
    SDL_Surface *surface = SDL_CreateSurfaceFrom(width, height, SDL_PIXELFORMAT_RGBA32, pixels, pitch);
    if (surface == nullptr)
    {
        logE("Failed to create surface: %s", SDL_GetError());
        free(pixels);
        return nullptr;
    }

    /* Create a texture from the surface */
    SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surface);
    if (texture == nullptr)
    {
        logE("Failed to create texture from surface, SDL_ERROR: %s.", SDL_GetError());
        SDL_DestroySurface(surface);
        free(pixels);
        return nullptr;
    }

    /* Clean up resources */
    SDL_DestroySurface(surface);
    free(pixels);

    return texture;
}

void
set_ren_color(SDL_Renderer *ren, SDL_Color *c) noexcept
{
    SDL_SetRenderDrawColor(ren, c->r, c->g, c->b, c->a);
}

