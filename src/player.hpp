#include <vector>
#include <array>
#include <SDL.h>

#pragma once

#define PLAYER_WIDTH 20
#define PLAYER_HEIGHT 28

class Player
{
public:
    Player(float px, float py);
    ~Player();

    void input(float timeStep, const uint8_t *state, std::vector<SDL_Rect*> cbox_vec);
    void move(int velY, int velX, float timeStep, std::vector<SDL_Rect*> cbox_vec);
    void render(SDL_Renderer *rend, SDL_Rect *cam);

    std::array<float, 2> getPos();
    SDL_Rect* get_cbox_ref();

private:
    /* World position */
    float x, y;

    /*Collision Box */
    SDL_Rect cbox;

    /* Visual representation (while we dont have textures) */
    SDL_Rect rect;

};
