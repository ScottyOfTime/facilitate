#include "ecs/components/collider.hpp"

Collider rect_to_collider(SDL_Rect rect)
{
    Collider c;
    c.x = rect.x;
    c.y = rect.y;
    c.h = rect.h;
    c.w = rect.w;

    return c;
}

SDL_Rect collider_to_rect(Collider col)
{
    SDL_Rect r;
    r.x = col.x;
    r.y = col.y;
    r.h = col.h;
    r.w = col.w;

    return r;
}
