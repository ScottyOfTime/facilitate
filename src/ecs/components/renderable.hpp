#include <SDL.h>
#include "texture.hpp"
#pragma once

typedef struct Renderable {
    SDL_Rect rect;
    Texture *tex;
} Renderable;
