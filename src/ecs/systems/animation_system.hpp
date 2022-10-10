#include "ecs/system.hpp"
#include "ecs/coordinator.hpp"

#pragma once

class AnimationSystem : public System
{
public:
    void init();

    void update(float dt);
};
