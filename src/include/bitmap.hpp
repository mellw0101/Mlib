#include "Debug.h"
#include "def.h"
#include <csetjmp>
#include <cstdint>
#include <cstdio>
#include <png.h>

#include <pngconf.h>
#include <vector>

class Bitmap
{
private:
    /** @c Variabels */
    int width, height;
    std::vector<std::vector<bool>>(bitmap);

public:
    /** @c Methods   */
    void modify(int row, int startCol, int endCol, bool value)
    {
        if (row < 0 || row >= height || startCol < 0 || endCol > width)
        {
            logE("Invalid row or column indices");
        }

        for (int i = startCol; i < endCol; ++i)
        {
            bitmap[row][i] = value;
        }
    }

    void exportToPng(const char* file_name) const
    {
        FILE* fp = fopen(file_name, "wb");
        if (!fp)
        {
            logE("Failed to create PNG file");
            return;
        }

        png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if (!png_ptr)
        {
            fclose(fp);
            logE("Failed to create PNG write struct");
            return;
        }

        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr)
        {
            fclose(fp);
            png_destroy_write_struct(&png_ptr, nullptr);
            logE("Failed to create PNG info struct");
            return;
        }

        if (setjmp(png_jmpbuf(png_ptr)))
        {
            fclose(fp);
            png_destroy_write_struct(&png_ptr, &info_ptr);
            logE( "Error during PNG creation");
            return;
        }

        png_init_io(png_ptr, fp);
        png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
                     PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
        png_write_info(png_ptr, info_ptr);

        png_bytep row = new png_byte[width];
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                row[x] = bitmap[y][x] ? 0xFF : 0x00;
            }

            png_write_row(png_ptr, row);
        }

        delete[] row;
        png_write_end(png_ptr, nullptr);
        fclose(fp);
        png_destroy_write_struct(&png_ptr, &info_ptr);
    }

    Bitmap(int width, int height) : width(width), height(height), bitmap(height, std::vector<bool>(width, false))
    {}
};

class color_bitmap
{
private:
    /* Variabels   */
    int                   width, height;
    std::vector<uint32_t> pixels; // Pixel data in ARGB format

public:
    /* Constructor */
    color_bitmap(int width, int height, const std::vector<uint32_t>& pixels)
        : width(width), height(height), pixels(pixels)
    {}

    /* Methods     */
    void exportToPng(const char* fileName) const
    {
        FILE* fp = fopen(fileName, "wb");
        if (!fp)
        {
            logE("Could not open file: %s", fileName);
            return;
        }
        png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if (!png_ptr)
        {
            fclose(fp);
            logE("Could not create write struct");
            return;
        }
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr)
        {
            fclose(fp);
            png_destroy_write_struct(&png_ptr, nullptr);
            logE("Could not create info struct");
            return;
        }
        if (setjmp(png_jmpbuf(png_ptr)))
        {
            fclose(fp);
            png_destroy_write_struct(&png_ptr, &info_ptr);
            logE("setjmp failed");
            return;
        }
        png_init_io(png_ptr, fp);
        // Note: Using PNG_COLOR_TYPE_RGB_ALPHA to include alpha channel
        png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
                     PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
        png_write_info(png_ptr, info_ptr);
        // Prepare row pointers
        png_bytep row_pointers[height];
        for (int y = 0; y < height; y++)
        {
            row_pointers[y] = (png_bytep)&pixels[y * width];
        }
        png_write_image(png_ptr, row_pointers);
        png_write_end(png_ptr, nullptr);
        fclose(fp);
        png_destroy_write_struct(&png_ptr, &info_ptr);
    }
};