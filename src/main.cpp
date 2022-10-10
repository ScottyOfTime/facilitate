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

const int WIN_HEIGHT = 480;
const int WIN_WIDTH = 640;

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

    Tilemap tilemap;
    tilemap.load_tilemap_from_file("/home/scotty/plgr/cpp/facilitate/tileset.png", 64, 64, rend);
    tilemap.register_tile("00_blank", 0, 0);
    tilemap.register_tile("01_uselessWall", 64, 0);
    tilemap.register_tile("02_floorTopRight", 128, 0);
    tilemap.register_tile("03_floorBottomLeft", 192, 0);
    tilemap.register_tile("04_floorBottomRight", 256, 0);
    tilemap.register_tile("05_floorTopLeft", 320, 0);
    tilemap.register_tile("06_floorCentre", 0, 64);
    tilemap.register_tile("07_floorCentreTop", 64, 64);
    tilemap.register_tile("08_floorCentreRight", 128, 64);
    tilemap.register_tile("09_floorCentreLeft", 192, 64);
    tilemap.register_tile("10_floorCentreBottom", 256, 64);
    tilemap.register_tile("11_roofPillar", 320, 64);
    tilemap.register_tile("12_roofVertTop", 0, 128);
    tilemap.register_tile("13_roofVertCentre", 64, 128);
    tilemap.register_tile("14_wall", 128, 128);
    tilemap.register_tile("15_roofHoriCentre", 192, 128);
    tilemap.register_tile("16_roofHoriRight", 256, 128);
    tilemap.register_tile("17_roofBottomRight", 320, 128);
    tilemap.register_tile("18_roofHoriLeft", 0, 192);
    tilemap.register_tile("19_roofTopLeft", 64, 192);
    tilemap.register_tile("20_roofBottomRight", 128, 192);
    tilemap.register_tile("21_roofTopRight", 192, 192);
    tilemap.register_tile("22_roofVertBottom", 256, 192);
    tilemap.register_tile("23_vine", 320, 192);

    Tilemap *tilemap_ptr = &tilemap;
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
    auto box = coordinator.create_entity();
    coordinator.add_component(box, Transform{
            .position = Vec2{120, 120},
            .scale = Vec2{1, 1}
            });
    coordinator.add_component(box, Velocity{0, 0});
    Collider col;
    col.x = 0;
    col.y = 0;
    col.w = 100;
    col.h = 100;
    SDL_Rect col_from_rect = collider_to_rect(col);
    coordinator.add_component(box, Renderable{
            .rect = &col_from_rect,
            .hasCollision = true,
            .collision = &col_from_rect});
    coordinator.add_component(box, col);

    auto ghost_box = coordinator.create_entity();
    coordinator.add_component(ghost_box, Transform{
            .position = Vec2{250, 250},
            .scale = Vec2{1, 1}
            });
    SDL_Rect ghost_rect = SDL_Rect{200, 200, 64, 64};
    coordinator.add_component(ghost_box, Renderable{
            .rect = &ghost_rect
            });
    
    auto player = coordinator.create_entity();
    coordinator.add_component(player, Transform{
            .position = Vec2{60, 60},
            .scale = Vec2{1, 1}
            });
    coordinator.add_component(player, Velocity{0, 0});
    r.w = 52;
    r.h = 50;
    r.x = 0;
    r.y = 60;
    Texture playerTexture;
    uint8_t res = playerTexture.load_from_file("/home/scotty/plgr/cpp/facilitate/dis.png", rend);
    printf("Generating sprite clips\n");
    SDL_Rect spriteClips[14];
    for (int i = 0; i < 14; i++) {
        SDL_Rect clip;
        clip.w = 26;
        clip.h = 55;
        clip.x = 29 * i;
        clip.y = 0;
        spriteClips[i] = clip;
    }
    AnimatedSprite playerSprite;
    playerSprite.create_animation("01_Idle", &playerTexture, 14, 29, 55, 100, 0);
    playerSprite.play_animation("01_Idle", true);
    coordinator.add_component(player, playerSprite);
    r.w = 52;
    r.h = 30;
    r.x = 0;
    r.y = 80;
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
        //tilemap.render_tile("02_floorTopRight", 150, 150, rend, &cam);
        renderSystem->update(timeStep);

        SDL_Delay(1);
    }
    return 0;
}

Level construct_sample_level(Tilemap* tmap)
{
    Level l;
    l.add_tile(0, 0, 1, tmap, "19_roofTopLeft");
    l.add_tile(64 * 2, 0, 1, tmap, "15_roofHoriCentre");
    l.add_tile(128 * 2, 0, 1, tmap, "15_roofHoriCentre");
    l.add_tile(196 * 2, 0, 1, tmap, "15_roofHoriCentre");
    l.add_tile(256 * 2, 0, 1, tmap, "21_roofTopRight");
    l.add_tile(0, 64 * 2, 1, tmap, "13_roofVertCentre");
    l.add_tile(64 * 2, 64 * 2, 1, tmap, "14_wall");
    l.add_tile(128 * 2, 64 * 2, 1, tmap, "14_wall");
    l.add_tile(192 * 2, 64 * 2, 1, tmap, "14_wall");
    l.add_tile(256 * 2, 64 * 2, 1, tmap, "13_roofVertCentre");
    return l;
}
