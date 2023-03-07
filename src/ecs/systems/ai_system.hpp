#include "ecs/system.hpp"
#include "ecs/coordinator.hpp"

#pragma once

class AI_System : public system
{
public:
    void init();

    void update(float dt);
}
