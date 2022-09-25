#include <SDL.h>

#pragma once

typedef struct Collider
{
    int w, h;
    float x, y;
} Collider;

Collider rect_to_collider(SDL_Rect rect)
{
    Collider c;
    c.w = rect.w;
    c.h = rect.h;
    c.x = rect.x;
    c.y = rect.y;

    return c;
}
