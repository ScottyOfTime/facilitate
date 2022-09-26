#include "ecs/systems/physics_system.hpp"
#include "ecs/components/transform.hpp"
#include "ecs/components/velocity.hpp"
#include "ecs/coordinator.hpp"

extern Coordinator coordinator;

void PhysicsSystem::update(float dt)
{
    for (auto const& ent : entities)
    {
        auto& transform = coordinator.get_component<Transform>(ent);
        auto& velocity = coordinator.get_component<Velocity>(ent);

        //printf("Physics system: Entity %d at (%f, %f); dt = %f\n", ent, transform.position.x,
                //transform.position.y, dt);
        //printf("Physics system: Entity %d velocity at (%f, %f)\n", ent, velocity.vel.x,
                //velocity.vel.y);

        transform.position.x += velocity.x * dt;
        transform.position.y += velocity.y * dt;
        //printf("Changed entity %d, position to (%d, %d)\n", ent, transform.position.x,
                //transform.position.y);
    }
}
