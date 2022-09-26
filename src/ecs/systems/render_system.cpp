#include "ecs/systems/render_system.hpp"
#include "ecs/components/renderable.hpp"
#include "ecs/components/transform.hpp"
#include "ecs/coordinator.hpp"

extern Coordinator coordinator;

void RenderSystem::init(SDL_Renderer *renderer, SDL_Rect *camera)
{
    rend = renderer;
    cam = camera;
}

void RenderSystem::update(float dt)
{
    SDL_SetRenderDrawColor(rend, 62, 73, 92, 255);
    SDL_RenderClear(rend);
    for (auto const& ent : entities) {
        auto& renderable = coordinator.get_component<Renderable>(ent);
        auto& transform = coordinator.get_component<Transform>(ent);


        printf("Rendering at (%f, %f)\n", transform.position.x, transform.position.y);

        SDL_Rect rr;
        rr.w = renderable.rect.w;
        rr.h = renderable.rect.h;
        rr.x = (int)transform.position.x - cam->x;
        rr.y = (int)transform.position.y - cam->y;
        
        SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
        SDL_RenderFillRect(rend, &rr);
        SDL_RenderDrawRect(rend, &rr);
    }
    SDL_RenderPresent(rend);
}
