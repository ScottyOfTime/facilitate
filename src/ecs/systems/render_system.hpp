#include <SDL.h>
#include "ecs/system.hpp"

#pragma once

class RenderSystem : public System
{
public:
    void init(SDL_Renderer *renderer, SDL_Rect *cam);

    void update(float dt);

private:
    SDL_Renderer *rend;
    SDL_Rect *cam;
};
