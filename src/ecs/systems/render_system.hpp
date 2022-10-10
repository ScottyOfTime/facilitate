#include <SDL.h>
#include "ecs/system.hpp"
#include "ecs/components/collider.hpp"
#include "tilemap.hpp"
#include "level.hpp"

#pragma once

class RenderSystem : public System
{
public:
    void init(SDL_Renderer *renderer, SDL_Rect *cam, Level *level = NULL);

    void update(float dt);

private:
    SDL_Renderer *rend;
    SDL_Rect *cam;
    Level *lvl;
};
