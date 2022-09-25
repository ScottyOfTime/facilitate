#include "ecs/system.hpp"

#pragma once

class PhysicsSystem : public System
{
public:
    void init();

    void update(float dt);
};

