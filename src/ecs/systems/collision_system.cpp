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

        collider.x = transform.position.x;
        collider.y = transform.position.y;

        /* Iterate through all other entities with collision signature O(n^2) */
        for (auto const& entOther : entities)
        {
            if (ent != entOther) {
                printf("Collision system: Checking Entity %d against Entity %d\n", ent, entOther);
                auto& colliderOther = coordinator.get_component<Collider>(entOther);
                printf("(%d, %d, %f, %f), (%d, %d, %f, %f)\n",
                        collider.w, collider.h, collider.x, collider.y,
                        colliderOther.w, colliderOther.h, colliderOther.x, colliderOther.y);
                /* COLLISION HANDLING
                 *
                 * Two collision checks are dont to seperate axes and allow 
                 * sliding along objects. 
                 */
                Collider r1;
                r1.w = collider.w;
                r1.h = collider.h;
                r1.x = collider.x + (velocity.vel.x * dt);
                r1.y = collider.y;

                Collider r2;
                r2.w = collider.w;
                r2.h = collider.h;
                r2.x = collider.x;
                r2.y = collider.y + (velocity.vel.y * dt);
                if (check_collision(&r1, &colliderOther)) {
                    velocity.vel.x = 0;
                }
                if (check_collision(&r2, &colliderOther)) {
                    velocity.vel.y = 0;
                }
            }
        }
    }
}
