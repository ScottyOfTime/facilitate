#include <SDL.h>

#pragma once

class Object 
{
public:
    Object(float px, float py, int w, int h);
    ~Object();

    void render(SDL_Renderer *rend, SDL_Rect *cam);

    SDL_Rect* get_cbox_ref();

private:
    /* World position */
    float x, y;

    /* Collision box */
    SDL_Rect cbox;

    /* Visual box */
    SDL_Rect rect;
};
