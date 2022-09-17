/* Texture wrapper for hardware-accelerated image loading and rendering */
#include <SDL.h>
#include <SDL_image.h>

#pragma once

class Texture
{
public:
    Texture();
    ~Texture();

    uint8_t loadFromFile(char* path, SDL_Renderer* rend);
    void free();
    void render(int x, int y, SDL_Rect* clip = NULL);

private:
    SDL_Texture* tex;
    int w, h;
};
