#include <SDL.h>

#include "timer.hpp"

Timer::Timer()
{
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

Timer::~Timer() {}

void Timer::start_timer()
{
    started = true;
    paused = false;

    startTicks = SDL_GetTicks();
    pausedTicks = 0;
}

void Timer::stop_timer()
{
    started = false;
    paused = false;

    startTicks = 0;
    pausedTicks = 0;
}

void Timer::pause_timer()
{
    if (started && !paused) {
        paused = true;
        pausedTicks = SDL_GetTicks() - startTicks;
        startTicks = 0;
    }
}

void Timer::unpause_timer()
{
    if (started && paused) {
        paused = false;
        startTicks = SDL_GetTicks() - pausedTicks;
        pausedTicks = 0;
    }
}

uint32_t Timer::timer_get_ticks()
{
    if (started) {
        if (paused) {
            return pausedTicks;
        } else {
            return SDL_GetTicks() - startTicks;
        }
    }
	return 0;
}

bool Timer::is_started()
{
    return started;
}

bool Timer::is_paused()
{
    return paused;
}
