#include "ecs/systems/collision_system.hpp"
#include "ecs/components/transform.hpp"
#include "ecs/components/velocity.hpp"
#include "ecs/components/collider.hpp"
#include "ecs/coordinator.hpp"
#include "collision.hpp"

extern Coordinator coordinator;

void CollisionSystem::update(float dt)
{
    for (auto const& ent : entities)
    {
        auto& transform = coordinator.get_component<Transform>(ent);
        auto& velocity = coordinator.get_component<Velocity>(ent);
        auto& collider = coordinator.get_component<Collider>(ent);

        //collider.x = transform.position.x;
        //collider.y = transform.position.y;

        /* Iterate through all other entities with collision signature O(n^2) */
        for (auto const& entOther : entities)
        {
            if (ent != entOther) {
                //printf("Collision system: Checking Entity %d against Entity %d\n", ent, entOther);
                auto& colliderOther = coordinator.get_component<Collider>(entOther);
                auto& transformOther = coordinator.get_component<Transform>(entOther);
                //printf("(%d, %d, %f, %f), (%d, %d, %f, %f)\n",
                        //collider.w, collider.h, collider.x, collider.y,
                        //colliderOther.w, colliderOther.h, colliderOther.x, colliderOther.y);
                /* COLLISION HANDLING
                 *
                 * Two collision checks are dont to seperate axes and allow 
                 * sliding along objects. 
                 */
                SDL_Rect r1;
                r1.w = collider.w;
                r1.h = collider.h;
                r1.x = transform.position.x + collider.x + (velocity.x * dt);
                r1.y = transform.position.y + collider.y;

                SDL_Rect r2;
                r2.w = collider.w;
                r2.h = collider.h;
                r2.x = transform.position.x + collider.x;
                r2.y = transform.position.y + collider.y + (velocity.y * dt);
                
                SDL_Rect rOther;
                rOther.w = colliderOther.w;
                rOther.h = colliderOther.h;
                rOther.x = transformOther.position.x + colliderOther.x;
                rOther.y = transformOther.position.y + colliderOther.y;
                if (check_collision(&r1, &rOther)) {
                    velocity.x = 0;
                }
                if (check_collision(&r2, &rOther)) {
                    velocity.y = 0;
                }
            }
        }
    }
}
