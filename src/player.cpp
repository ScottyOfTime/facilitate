#include <stdio.h>

#include "player.hpp"
#include "collision.h"

#define PLAYER_SPEED 200

const float SIN_45 = 0.707106;

Player::Player(float px, float py)
{
    x = px;
    y = py;

    cbox.x = (int)px;
    cbox.y = (int)py + 20;
    cbox.w = PLAYER_WIDTH;
    cbox.h = PLAYER_HEIGHT - 20;

    rect.x = (int)px;
    rect.y = (int)py;
    rect.w = PLAYER_WIDTH;
    rect.h = PLAYER_HEIGHT;
}

Player::~Player() {};

void Player::input(float timeStep, const uint8_t *state, std::vector<SDL_Rect*> cbox_vec)
{
    //printf("pos x: %d, pos y: %d\n", x, y);
    int velY = 0, velX = 0;
    /* Updated collision box that is tested against */

    if (state[SDL_SCANCODE_W]) {
        velY -= PLAYER_SPEED;
    }
    if (state[SDL_SCANCODE_S]) {
        velY += PLAYER_SPEED;
    }
    if (state[SDL_SCANCODE_A]) {
        velX -= PLAYER_SPEED;
    }
    if (state[SDL_SCANCODE_D]) {
        velX += PLAYER_SPEED;
    }
    move(velX, velY, timeStep, cbox_vec);
}

void Player::move(int velX, int velY, float timeStep, std::vector<SDL_Rect*> cbox_vec)
{
    float n_x, n_y;
    if (velX && velY) {
        n_x = x + ((SIN_45 * (float)velX) * timeStep);
        n_y = y + ((SIN_45 * (float)velY) * timeStep);
    } else {
        n_x = x + ((float)velX * timeStep);
        n_y = y + ((float)velY * timeStep);
    }

    /* COLLISION HANDLING
     *
     * Two collisions checks are done to seperate axes and allow sliding along objects.
     * I don't know if this is the most efficient way but it works.
     * !!! PLEASE REVISIT THIS !!!
     */
    SDL_Rect r1;
    r1.w = cbox.w;
    r1.h = cbox.h;
    r1.x = (int)n_x;
    r1.y = cbox.y;

    SDL_Rect r2;
    r2.w = cbox.w;
    r2.h = cbox.h;
    r2.x = cbox.x;
    r2.y = (int)n_y + 20;

    if (!check_n_collisions(&r1, cbox_vec.data(), cbox_vec.size())) {
        x = n_x;
        cbox.x = (int)x;
        rect.x = (int)x;
    }
    if (!check_n_collisions(&r2, cbox_vec.data(), cbox_vec.size())) {
        y = n_y;
        cbox.y = (int)y + 20;
        rect.y = (int)y;
    }
}

void Player::render(SDL_Renderer *rend, SDL_Rect *cam)
{
    SDL_Rect rr;
    rr.w = PLAYER_WIDTH;
    rr.h = PLAYER_HEIGHT;
    rr.x = x - cam->x;
    rr.y = y - cam->y;

    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    SDL_RenderFillRect(rend, &rr);
    SDL_RenderDrawRect(rend, &rr);
}

std::array<float, 2> Player::getPos() 
{
    float posX = x;
    float posY = y;
    std::array<float, 2> a = {posX, posY};
    return a;
}

SDL_Rect* Player::get_cbox_ref()
{
    return &cbox;
}
