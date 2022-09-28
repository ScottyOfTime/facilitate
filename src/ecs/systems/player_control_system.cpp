#include <SDL.h>
#include "ecs/systems/player_control_system.hpp"
#include "ecs/components/velocity.hpp"
#include "ecs/coordinator.hpp"

#define PLAYER_SPEED 200
#define SIN_45 0.7071067812 

extern Coordinator coordinator;

void PlayerControlSystem::init(const uint8_t *keyboard_state)
{
    state = keyboard_state;
}

void PlayerControlSystem::update(float dt)
{
    for (auto const& ent : entities)
    {
        //printf("WE HAVE AN ENTITY\n");
        auto& velocity = coordinator.get_component<Velocity>(ent);

        velocity.x = 0;
        velocity.y = 0;

        if (state[SDL_SCANCODE_W]) {
            velocity.y -= PLAYER_SPEED;
        }
        if (state[SDL_SCANCODE_S]) {
            velocity.y += PLAYER_SPEED;
        }
        if (state[SDL_SCANCODE_A]) {
            velocity.x -= PLAYER_SPEED;
        }
        if (state[SDL_SCANCODE_D]) {
            velocity.x += PLAYER_SPEED;
        }

        if (velocity.x && velocity.y) {
            velocity.x = SIN_45 * velocity.x;
            velocity.y = SIN_45 * velocity.y;
        }
    }
}
