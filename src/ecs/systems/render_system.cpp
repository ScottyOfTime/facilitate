#include "ecs/systems/render_system.hpp"
#include "ecs/components/renderable.hpp"
#include "ecs/components/transform.hpp"
#include "ecs/coordinator.hpp"

extern Coordinator coordinator;

void RenderSystem::init(SDL_Renderer *renderer, SDL_Rect *camera, Level *level)
{
    rend = renderer;
    cam = camera;
    lvl = level;
    //SDL_RenderSetScale(rend, 1.3f, 1.3f);
}

void RenderSystem::update(float dt)
{
    SDL_SetRenderDrawColor(rend, 62, 73, 92, 255);
    SDL_RenderClear(rend);
    
    /* Always render level first */
    if (lvl != NULL) {
        lvl->render_level(rend, cam);
        lvl->render_tile_collision(rend, cam);
        //tmap->render_tile("14_wall", 64, 64, rend, cam);
        //tmap->render_tile("11_roofPillar", 64, -32, rend, cam);
    }

    for (auto const& ent : entities) {
        auto& renderable = coordinator.get_component<Renderable>(ent);
        auto& transform = coordinator.get_component<Transform>(ent);


        //printf("Rendering at (%f, %f)\n", transform.position.x, transform.position.y);

        
        /* If entity renderable has texture */
        if (renderable.tex != NULL) {
            SDL_Rect r;
            r.x = 0;
            r.y = 0;
            r.w = 45;
            r.h = 51;
            renderable.tex->render(transform.position.x - cam->x, transform.position.y - cam->y, 
                    rend, &renderable.clip, transform.scale);
        }
        /* If entity renderable has a rectangle */
        if (renderable.rect != NULL) {
            SDL_Rect rr;
            rr.w = renderable.rect->w;
            rr.h = renderable.rect->h;
            rr.x = transform.position.x - cam->x;
            rr.y = transform.position.y - cam->y;
            SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
            SDL_RenderFillRect(rend, &rr);
            SDL_RenderDrawRect(rend, &rr);
        }
        /* For debugging purposes (rendering the collision boxes) */
        if (renderable.hasCollision) {
            auto& collider = coordinator.get_component<Collider>(ent);
            SDL_Rect cr = collider_to_rect(collider);
            cr.x = collider.x + transform.position.x - cam->x;
            cr.y = collider.y + transform.position.y - cam->y;
            SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
            SDL_RenderDrawRect(rend, &cr);
        }
    }
    SDL_RenderPresent(rend);
}
