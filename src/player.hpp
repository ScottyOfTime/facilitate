#include <vector>
#include <SDL.h>

#pragma once

class Player
{
public:
    Player(float px, float py);
    ~Player();

    void input(float timeStep, const uint8_t *state, std::vector<SDL_Rect*> cbox_vec);
    void render(SDL_Renderer *rend);

    SDL_Rect* get_cbox_ref();

private:
    /* World position */
    float x, y;

    /*Collision Box */
    SDL_Rect cbox;

    /* Visual representation (while we dont have textures) */
    SDL_Rect rect;

};