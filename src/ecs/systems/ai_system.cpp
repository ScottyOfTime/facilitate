#include "ecs/systems/ai_system.hpp"

extern Coordinator coordinator;

void AI_System::init() {};

void AI_System::update(float dt)
{
    for (auto const &ent : entitites) {
        auto& aiComponent = coordinator.get_component<AIComponent>(ent);
    }
}
