#include <stdio.h>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>

#include "timer.hpp"
#include "animation.hpp"
#include "tilemap.hpp"
#include "level.hpp"
#include "ecs/coordinator.hpp"
#include "ecs/components/renderable.hpp"
#include "ecs/components/transform.hpp"
#include "ecs/components/velocity.hpp"
#include "ecs/components/collider.hpp"
#include "ecs/components/player.hpp"
#include "ecs/systems/physics_system.hpp"
#include "ecs/systems/render_system.hpp"
#include "ecs/systems/player_control_system.hpp"
#include "ecs/systems/collision_system.hpp"
#include "ecs/systems/animation_system.hpp"

const int WIN_HEIGHT = 720;
const int WIN_WIDTH = 1280;

int game_loop(SDL_Renderer *rend);
Level construct_sample_level(Tilemap *tmap);

Coordinator coordinator;

int main(int argc, char *argv[])
{
    SDL_Window *win;
    SDL_Renderer *rend;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Could not initialize SDL video subsystem, SDL Error: %s\n",
            SDL_GetError());
            return 1;
    }
    win = SDL_CreateWindow("Facilitate", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        WIN_WIDTH, WIN_HEIGHT, 0);
    if (win == NULL) {
        fprintf(stderr, "Could not create SDL window, SDL Error: %s\n",
            SDL_GetError());
        return 1;
    }
    rend = SDL_CreateRenderer(win, -1, 0);
    if (rend == NULL) {
        fprintf(stderr, "Could not create SDL renderer, SDL Error: %s\n",
            SDL_GetError());
        return 1;
    }
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        fprintf(stderr, "Could not initialize! SDL_image Error: %s\n",
                IMG_GetError());
        return 1;
    }
    
    SDL_SetRenderDrawColor(rend, 62, 73, 92, 255);
    SDL_RenderClear(rend);
    SDL_RenderPresent(rend);
    
    game_loop(rend);

    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

int game_loop(SDL_Renderer *rend)
{
    // Make the camera
    SDL_Rect cam;
    cam.x = 0;
    cam.y = 0;
    cam.w = WIN_WIDTH;
    cam.h = WIN_HEIGHT;

    Timer t = Timer();
    float timeStep;
    const uint8_t *state = SDL_GetKeyboardState(NULL);

    /* BEGIN TILEMAP STUFF */

    Tilemap sandMap;

    sandMap.load_tilemap_from_file("assets/sandMap.png", 56, 56, rend);
    sandMap.register_tile("00_blank", 0, 0);
    sandMap.register_tile("01_wall", 56, 0);
    sandMap.register_tile("02_roofHori", 112, 0);
    sandMap.register_tile("03_roofHoriLeft", 168, 0);
    sandMap.register_tile("04_roofHoriRight", 0, 56);
    sandMap.register_tile("05_roofCornerLeftT", 56, 56);
    sandMap.register_tile("06_roofCornerRightT", 112, 56);
    sandMap.register_tile("07_roofVert", 168, 56);
    sandMap.register_tile("08_roofVertBottom", 0, 112);
    sandMap.register_tile("09_roofCornerLeftB", 56, 112);
    sandMap.register_tile("10_roofCornerRightB", 112, 112);
    sandMap.register_tile("11_roofVertTop", 168, 112);
    sandMap.register_tile("12_roofPillar", 0, 168);
    sandMap.register_tile("13_floor", 56, 168);

    Tilemap *tilemap_ptr = &sandMap;
    Level level = construct_sample_level(tilemap_ptr);
    /* END TILEMAP STUFF */

    /* ECS STUFF */
    coordinator.init();

    coordinator.register_component<Transform>();
    coordinator.register_component<Velocity>();
    coordinator.register_component<Renderable>();
    coordinator.register_component<AnimatedSprite>();
    coordinator.register_component<Collider>();
    coordinator.register_component<Player>();

    /* PHYSICS SYSTEM SETUP */
    auto physicsSystem = coordinator.register_system<PhysicsSystem>();
    Signature signature;
    signature.set(coordinator.get_component_type<Transform>());
    signature.set(coordinator.get_component_type<Velocity>());
    coordinator.set_system_signature<PhysicsSystem>(signature);

    /* COLLISION SYSTEM SETUP */
    auto collisionSystem = coordinator.register_system<CollisionSystem>();
    collisionSystem->init(&level);
    signature.reset();
    signature.set(coordinator.get_component_type<Transform>());
    signature.set(coordinator.get_component_type<Velocity>());
    signature.set(coordinator.get_component_type<Collider>());
    coordinator.set_system_signature<CollisionSystem>(signature);

    /* RENDER SYSTEM SETUP */
    auto renderSystem = coordinator.register_system<RenderSystem>();
    renderSystem->init(rend, &cam, &level);
    signature.reset();
    signature.set(coordinator.get_component_type<Transform>());
    signature.set(coordinator.get_component_type<Renderable>());
    coordinator.set_system_signature<RenderSystem>(signature);

    /* ANIMATION SYSTEM SETUP */
    auto animationSystem = coordinator.register_system<AnimationSystem>();
    signature.reset();
    signature.set(coordinator.get_component_type<AnimatedSprite>());
    signature.set(coordinator.get_component_type<Renderable>());
    coordinator.set_system_signature<AnimationSystem>(signature);

    /* PLAYER SYSTEM SETUP */
    auto playerSystem = coordinator.register_system<PlayerControlSystem>();
    playerSystem->init(state);
    signature.reset();
    signature.set(coordinator.get_component_type<Transform>());
    signature.set(coordinator.get_component_type<Renderable>());
    signature.set(coordinator.get_component_type<Velocity>());
    signature.set(coordinator.get_component_type<Player>());
    coordinator.set_system_signature<PlayerControlSystem>(signature);


    SDL_Rect r;
    r.w = 52;
    r.h = 50;
    r.x = 0;
    r.y = 60;
    
    /* PLAYER ENTITY CREATION */
    auto player = coordinator.create_entity();
    coordinator.add_component(player, Transform{
            .position = Vec2{160, 160},
            .scale = Vec2{2, 2}
            });
    coordinator.add_component(player, Velocity{0, 0});
    r.w = 52;
    r.h = 50;
    r.x = 0;
    r.y = 60;
    Texture playerTexture;
    Texture playerWalkingTexture;
    uint8_t res = playerTexture.load_from_file("assets/dis.png", rend);
    uint8_t resW = playerWalkingTexture.load_from_file("assets/character_walking.png", rend);
    AnimatedSprite playerSprite;
    playerSprite.create_animation("01_Idle", &playerTexture, 14, 29, 60, 100);
    playerSprite.play_animation("01_Idle", true);
    playerSprite.create_animation("02_Walking_S", &playerWalkingTexture, 6, 29, 60, 100, 0);
    playerSprite.create_animation("02_Walking_SE", &playerWalkingTexture, 6, 29, 60, 100, 60);
    playerSprite.create_animation("02_Walking_E", &playerWalkingTexture, 6, 29, 60, 100, 120);
    playerSprite.create_animation("02_Walking_NE", &playerWalkingTexture, 6, 29, 60, 100, 180);
    playerSprite.create_animation("02_Walking_N", &playerWalkingTexture, 6, 29, 60, 100, 240);
    playerSprite.create_animation("02_Walking_NW", &playerWalkingTexture, 6, 29, 60, 100, 300);
    playerSprite.create_animation("02_Walking_W", &playerWalkingTexture, 6, 29, 60, 100, 360);
    playerSprite.create_animation("02_Walking_SW", &playerWalkingTexture, 6, 29, 60, 100, 420);
    //playerSprite.play_animation("02_Walking", true);

    coordinator.add_component(player, playerSprite);
    r.w = 52;
    r.h = 30;
    r.x = 0;
    r.y = 82;
    Collider coll = rect_to_collider(r);
    coordinator.add_component(player, Renderable{
            .tex = &playerTexture,
            .hasCollision = true,
            .collision = &r});
    coordinator.add_component(player, coll);
    coordinator.add_component(player, Player{1});

    /* END ECS STUFF */


    std::vector<Entity> entities(MAX_ENTITIES);

    SDL_Event e;
    bool quit = false;
    
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            switch(e.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
            }
        }
        timeStep = (float)t.timer_get_ticks() / 1000.f;

        playerSystem->update(timeStep);
        collisionSystem->update(timeStep);
        physicsSystem->update(timeStep);
        cam.x = ((int)coordinator.get_component<Transform>(player).position.x
                + 20 / 2) - WIN_WIDTH / 2;
        cam.y = ((int)coordinator.get_component<Transform>(player).position.y 
                + 20 / 2) - WIN_HEIGHT / 2;

        t.start_timer();
        
        animationSystem->update(timeStep);
        renderSystem->update(timeStep);

        SDL_Delay(1);
    }
    return 0;
}

Level construct_sample_level(Tilemap* tmap)
{
    Level l;
    /* Test level (square room)*/
    /*l.add_tile(0, 0, 1, tmap, "05_roofCornerLeftT");
    l.add_tile(56 * 2, 0, 1, tmap, "02_roofHori");
    l.add_tile(112 * 2, 1, 1, tmap, "02_roofHori");
    l.add_tile(168 * 2, 1, 1, tmap, "02_roofHori");
    l.add_tile(224 * 2, 0, 1, tmap, "06_roofCornerRightT");

    l.add_tile(0, 56 * 2, 1, tmap, "07_roofVert");
    l.add_tile(56 * 2, 56 * 2, 1, tmap, "01_wall");
    l.add_tile(112 * 2, 56 * 2, 1, tmap, "01_wall");
    l.add_tile(168 * 2, 56 * 2, 1, tmap, "01_wall");
    l.add_tile(224 * 2, 56 * 2, 1, tmap, "07_roofVert");

    l.add_tile(0, 112 * 2, 1, tmap, "07_roofVert");
    l.add_tile(56 * 2, 112 * 2, 0, tmap, "13_floor");
    l.add_tile(112 * 2, 112 * 2, 0, tmap, "13_floor");
    l.add_tile(168 * 2, 112 * 2, 0, tmap, "13_floor");
    l.add_tile(224 * 2, 112 * 2, 1, tmap, "07_roofVert");

    l.add_tile(0, 168 * 2, 1, tmap, "07_roofVert");
    l.add_tile(56 * 2, 168 * 2, 0, tmap, "13_floor");
    l.add_tile(112 * 2, 168 * 2, 0, tmap, "13_floor");
    l.add_tile(168 * 2, 168 * 2, 0, tmap, "13_floor");
    l.add_tile(224 * 2, 168 * 2, 1, tmap, "07_roofVert");

    l.add_tile(0, 224 * 2, 1, tmap, "07_roofVert");
    l.add_tile(56 * 2, 224 * 2, 0, tmap, "13_floor");
    l.add_tile(112 * 2, 224 * 2, 0, tmap, "13_floor");
    l.add_tile(168 * 2, 224 * 2, 0, tmap, "13_floor");
    l.add_tile(224 * 2, 224 * 2, 1, tmap, "07_roofVert");

    l.add_tile(0, 280 * 2, 1, tmap, "09_roofCornerLeftB");
    l.add_tile(56 * 2, 280 * 2, 1, tmap, "02_roofHori");
    l.add_tile(112 * 2, 280 * 2, 1, tmap, "02_roofHori");
    l.add_tile(168 * 2, 280 * 2, 1, tmap, "02_roofHori");
    l.add_tile(224 * 2, 280 * 2, 1, tmap, "10_roofCornerRightB");*/

    l.load_level_from_file("levels/1.lvl", tmap);
    return l;
}
