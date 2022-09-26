#include <SDL.h>

#pragma once

typedef struct Collider
{
    int w, h;
    float x, y;
} Collider;

Collider rect_to_collider(SDL_Rect rect);

SDL_Rect collider_to_rect(Collider col);
