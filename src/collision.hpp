#include <SDL.h>
#include "ecs/components/collider.hpp"

#pragma once

uint8_t check_collision(SDL_Rect *a, SDL_Rect *b);

uint8_t check_n_collisions(SDL_Rect *a, SDL_Rect **b, int n);
