#include <stdio.h>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>

#include "timer.hpp"
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

const int WIN_HEIGHT = 480;
const int WIN_WIDTH = 640;

int game_loop(SDL_Renderer *rend);

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


    /* ECS STUFF */
    coordinator.init();

    coordinator.register_component<Transform>();
    coordinator.register_component<Velocity>();
    coordinator.register_component<Renderable>();
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
    renderSystem->init(rend, &cam);
    signature.reset();
    signature.set(coordinator.get_component_type<Transform>());
    signature.set(coordinator.get_component_type<Renderable>());
    coordinator.set_system_signature<RenderSystem>(signature);

    /* PLAYER SYSTEM SETUP */
    auto playerSystem = coordinator.register_system<PlayerControlSystem>();
    playerSystem->init(state);
    signature.reset();
    signature.set(coordinator.get_component_type<Transform>());
    signature.set(coordinator.get_component_type<Renderable>());
    signature.set(coordinator.get_component_type<Velocity>());
    signature.set(coordinator.get_component_type<Player>());
    coordinator.set_system_signature<PlayerControlSystem>(signature);

    auto player = coordinator.create_entity();
    coordinator.add_component(player, Transform{
            .position = Vec2{60, 60},
            .scale = Vec2{1, 1}
            });
    coordinator.add_component(player, Velocity{0, 0});
    SDL_Rect r;
    r.w = 45;
    r.h = 51;
    r.x = 60;
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
    playerTexture.set_sheet(spriteClips, 14);

    coordinator.add_component(player, Renderable{
            .rect = r,
            .tex = &playerTexture});
    Collider col = rect_to_collider(r);
    coordinator.add_component(player, col);
    coordinator.add_component(player, Player{1});

    auto box = coordinator.create_entity();
    coordinator.add_component(box, Transform{
            .position = Vec2{120, 120},
            .scale = Vec2{1, 1}
            });
    coordinator.add_component(box, Velocity{0, 0});
    col.x = 120;
    col.y = 120;
    col.w = 100;
    col.h = 100;
    SDL_Rect col_from_rect = collider_to_rect(col);
    coordinator.add_component(box, Renderable{
            .rect = col_from_rect});
    coordinator.add_component(box, col);

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

        playerTexture.render(180, 180, rend);
        renderSystem->update(timeStep);

        SDL_Delay(1);
    }
    return 0;
}
