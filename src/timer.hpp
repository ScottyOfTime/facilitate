#pragma once

class Timer 
{
public:
    Timer();
    ~Timer();

    void start_timer();
    void stop_timer();
    void pause_timer();
    void unpause_timer();

    uint32_t timer_get_ticks();
    // getters here if needed
    bool is_started();
    bool is_paused();

private:
    uint32_t startTicks;
    uint32_t pausedTicks;
    bool paused;
    bool started;
};
