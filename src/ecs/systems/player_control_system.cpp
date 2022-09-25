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
        printf("WE HAVE AN ENTITY\n");
        auto& velocity = coordinator.get_component<Velocity>(ent);
        Vec2& vel = velocity.vel;

        vel.x = 0;
        vel.y = 0;

        if (state[SDL_SCANCODE_W]) {
            vel.y -= PLAYER_SPEED;
        }
        if (state[SDL_SCANCODE_S]) {
            vel.y += PLAYER_SPEED;
        }
        if (state[SDL_SCANCODE_A]) {
            vel.x -= PLAYER_SPEED;
        }
        if (state[SDL_SCANCODE_D]) {
            vel.x += PLAYER_SPEED;
        }

        if (vel.x && vel.y) {
            vel.x = SIN_45 * vel.x;
            vel.y = SIN_45 * vel.y;
        }
    }
}
