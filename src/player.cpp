#include <stdio.h>

#include "player.hpp"
#include "collision.h"

#define PLAYER_WIDTH 20
#define PLAYER_HEIGHT 28
#define PLAYER_VELOCITY 200

Player::Player(float px, float py)
{
    x = px;
    y = py;

    cbox.x = (int)px;
    cbox.y = (int)py;
    cbox.w = PLAYER_WIDTH;
    cbox.h = PLAYER_HEIGHT;

    rect.x = (int)px;
    rect.y = (int)py;
    rect.w = PLAYER_WIDTH;
    rect.h = PLAYER_HEIGHT;
}

Player::~Player() {};

void Player::input(float timeStep, const uint8_t *state, std::vector<SDL_Rect*> cbox_vec)
{
    //printf("pos x: %d, pos y: %d\n", x, y);
    float n_y, n_x;
    float n = timeStep * PLAYER_VELOCITY;
    /* Updated collision box that is tested against */
    SDL_Rect r;
    r.w = PLAYER_WIDTH;
    r.h = PLAYER_HEIGHT;
    r.x = (int)x;
    r.y = (int)y;

    if (state[SDL_SCANCODE_W]) {
        n_y = y - n;
        r.y = (int)n_y;

        if (!check_n_collisions(&r, cbox_vec.data(), cbox_vec.size())) {
            y = n_y;
            cbox.y = (int)y;
            rect.y = (int)y;
        }
    }
    if (state[SDL_SCANCODE_S]) {
        n_y = y + n;
        r.y = (int)n_y;

        if (!check_n_collisions(&r, cbox_vec.data(), cbox_vec.size())) {
            y = n_y;
            cbox.y = (int)y;
            rect.y = (int)y;
        }
    }
    if (state[SDL_SCANCODE_A]) {
        n_x = x - n;
        r.x = (int)n_x;

        if (!check_n_collisions(&r, cbox_vec.data(), cbox_vec.size())) {
            x = n_x;
            cbox.x = (int)x;
            rect.x = (int)x;
        }
    }
    if (state[SDL_SCANCODE_D]) {
        n_x = x + n;
        r.x = (int)n_x;

        if (!check_n_collisions(&r, cbox_vec.data(), cbox_vec.size())) {
            x = n_x;
            cbox.x = (int)x;
            rect.x = (int)x;
        }
    }
}

void Player::render(SDL_Renderer *rend)
{
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    SDL_RenderFillRect(rend, &rect);
    SDL_RenderDrawRect(rend, &rect);
}

SDL_Rect* Player::get_cbox_ref()
{
    return &cbox;
}