#include <SDL.h>
#include "ecs/components/collider.hpp"

#pragma once

uint8_t check_collision(Collider *a, Collider *b)
{
    return (
        a->x < b->x + b->w &&
        b->x < a->x + a->w &&
        a->y < b->y + b->h &&
        b->y < a->y + a->h
    );
}

uint8_t check_n_collisions(Collider *a, Collider **b, int n)
{
    for (int i = 0; i < n; i++) {
        if (check_collision(a, b[i])) {
            return 1;
        }
    }
    return 0;
}
