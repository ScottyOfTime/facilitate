#include "animation.hpp"
#include "ecs/components/renderable.hpp"
#include "ecs/systems/animation_system.hpp"

extern Coordinator coordinator;

void AnimationSystem::init() {};

void AnimationSystem::update(float dt)
{
    for (auto const &ent : entities) {
        auto& animatedSprite = coordinator.get_component<AnimatedSprite>(ent);
        auto& renderable = coordinator.get_component<Renderable>(ent);
        animatedSprite.update_animation();
        renderable.clip = animatedSprite.get_current_cell();
    }
}
