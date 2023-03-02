/* Animation classes for storing, playing and managing animations */
#include <unordered_map>
#include <vector>
#include <string>
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
    void create_animation(std::string anim_name, Texture* tex, uint8_t num_cells, int cell_w, 
            int cell_h, uint32_t rate, int y_off = 0, uint8_t rows = 1, int cell_p = 0);
    void play_animation(std::string anim, bool lp);
    void stop_animation();
    void update_animation();
    SDL_Rect get_current_cell();
    Texture* get_current_texture();

    std::unordered_map<std::string, Animation> anims{};
    Animation currentAnim;
    std::string currentAnimName;
    bool playing;
    bool loop;
    Timer timer;
};

