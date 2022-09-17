#include "object.hpp"

Object::Object(float px, float py, int w, int h)
{
    x = px;
    y = py;

    cbox.x = (int)px;
    cbox.y = (int)py;
    cbox.w = w;
    cbox.h = h;

    rect.x = (int)px;
    rect.y = (int)py;
    rect.w = w;
    rect.h = h;
}

Object::~Object() {}

void Object::render(SDL_Renderer *rend, SDL_Rect *cam)
{
    SDL_Rect rr;
    rr.x = rect.x - cam->x;
    rr.y = rect.y - cam->y;
    rr.w = rect.w;
    rr.h = rect.h;

    SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
    SDL_RenderFillRect(rend, &rr);
    SDL_RenderDrawRect(rend, &rr);
}

SDL_Rect* Object::get_cbox_ref()
{
    return &cbox;
}
