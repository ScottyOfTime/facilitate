#include <stdio.h>
#include <vector>
#include <SDL.h>

#include "player.hpp"
#include "timer.hpp"
#include "object.hpp"

const int WIN_HEIGHT = 1080;
const int WIN_WIDTH = 1920;

int game_loop(SDL_Renderer *rend);

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
    Player *p = new Player(64, 64);
    Timer t = Timer();
    float timeStep;
    const uint8_t *state = SDL_GetKeyboardState(NULL);
    std::vector<SDL_Rect*> collisionVec;

    Object box1 = Object(100, 100, 128, 128);
    collisionVec.push_back(box1.get_cbox_ref());

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

        p->input(timeStep, state, collisionVec);

        t.start_timer();

        SDL_SetRenderDrawColor(rend, 62, 73, 92, 255);
        SDL_RenderClear(rend);
        p->render(rend);
        box1.render(rend);
        SDL_RenderPresent(rend);
        SDL_Delay(1);
    }
    return 0;
}