/* Texture wrapper for hardware-accelerated image loading and rendering */
#include <SDL.h>
#include <SDL_image.h>
#include "timer.hpp"

#pragma once

class Texture
{
public:
    Texture();
    ~Texture();

    uint8_t load_from_file(const char *path, SDL_Renderer *rend);
    void render(int x, int y, SDL_Renderer *rend, SDL_Rect* clip = NULL);
    void free();

private:
    SDL_Texture *tex;
    int w, h;
};
