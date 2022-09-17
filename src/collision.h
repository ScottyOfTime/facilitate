#include <SDL.h>

uint8_t check_collision(SDL_Rect *a, SDL_Rect *b)
{
    return (
        a->x < b->x + b->w &&
        b->x < a->x + a->w &&
        a->y < b->y + b->h &&
        b->y < a->y + a->h
    );
}

uint8_t check_n_collisions(SDL_Rect *a, SDL_Rect **b, int n)
{
    for (int i = 0; i < n; i++) {
        if (check_collision(a, b[i])) {
            return 1;
        }
    }
    return 0;
}