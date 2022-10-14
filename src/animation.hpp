/* Animation classes for storing, playing and managing animations */
#include <unordered_map>
#include <vector>
#include "texture.hpp"
#include "timer.hpp"

#pragma once

typedef struct Animation {
    Texture* sheet;
    std::vector<SDL_Rect> cells;
    uint8_t num_cells;
    int frame;
    int rate;
} Animation;

class AnimatedSprite
{
public:
    void create_animation(const char* anim_name, Texture* tex, uint8_t num_cells, int cell_w, 
            int cell_h, uint32_t rate, int cell_p = 0);
    void play_animation(const char* anim, bool lp);
    void stop_animation();
    void update_animation();
    SDL_Rect get_current_cell();

    std::unordered_map<const char*, Animation> anims{};
    Animation currentAnim;
    bool playing;
    bool loop;
    Timer timer;
};

