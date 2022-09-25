#include "ecs/system.hpp"

#pragma once

class PlayerControlSystem : public System
{
public:
    void init(const uint8_t *keyboard_state);

    void update(float dt);

private:
    const uint8_t *state;
};
