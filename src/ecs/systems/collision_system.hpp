#include "ecs/system.hpp"
#include "level.hpp"

#pragma once

class CollisionSystem : public System
{
public:
    void init(Level *level);

    void update(float dt);

private:
    Level *lvl;
};
