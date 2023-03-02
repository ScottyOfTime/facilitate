#include "animation.hpp"
#include "ecs/components/renderable.hpp"
#include "ecs/components/velocity.hpp"
#include "ecs/systems/animation_system.hpp"

extern Coordinator coordinator;

void AnimationSystem::init() {};

void AnimationSystem::update(float dt)
{
    for (auto const &ent : entities) {
        auto& animatedSprite = coordinator.get_component<AnimatedSprite>(ent);
        auto& renderable = coordinator.get_component<Renderable>(ent);
        auto& velocity = coordinator.get_component<Velocity>(ent);
        if (velocity.x == 0 && velocity.y == 0) { 
            animatedSprite.play_animation("01_Idle", true); 
        } else if (velocity.x > 0 && velocity.y == 0) {
            animatedSprite.play_animation("02_Walking_E", true);
        } else if (velocity.x < 0 && velocity.y == 0) {
            animatedSprite.play_animation("02_Walking_W", true);
        } else if (velocity.x == 0 && velocity.y < 0) {
            animatedSprite.play_animation("02_Walking_N", true);
        } else if (velocity.x == 0 && velocity.y > 0) {
            animatedSprite.play_animation("02_Walking_S", true);
        } else if (velocity.x > 0 && velocity.y < 0 ) {
            animatedSprite.play_animation("02_Walking_NE", true);
        } else if (velocity.x > 0 && velocity.y > 0) {
            animatedSprite.play_animation("02_Walking_SE", true);
        } else if (velocity.x < 0 && velocity.y < 0) {
            animatedSprite.play_animation("02_Walking_NW", true);
        } else if (velocity.x < 0 && velocity.y > 0) {
            animatedSprite.play_animation("02_Walking_SW", true);
        }   
        animatedSprite.update_animation();
        renderable.tex = animatedSprite.get_current_texture();
        renderable.clip = animatedSprite.get_current_cell();
    }
}
