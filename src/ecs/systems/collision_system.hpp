#include "ecs/system.hpp"

#pragma once

class CollisionSystem : public System
{
    public:
        void init();

        void update(float dt);
};
