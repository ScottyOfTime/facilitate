#include <SDL.h>
#include "texture.hpp"
#pragma once

typedef struct Renderable {
    // The base level
    SDL_Rect *rect;
    Texture *tex;
    SDL_Rect clip;
    // Debug extension
    bool hasCollision;
    SDL_Rect *collision;
} Renderable;
