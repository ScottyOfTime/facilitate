#include "animation.hpp"

void AnimatedSprite::create_animation(const char* anim_name, Texture* tex, uint8_t num_cells,
        int cell_w, int cell_h, uint32_t rate, int cell_p)
{
    Animation anim;
    std::vector<SDL_Rect> cells;
    int padding = 0;
    for (int i = 0; i < num_cells; i++) {
        cells.push_back(SDL_Rect{
                .x = (i * cell_w) + padding,
                .y = 0,
                .w = cell_w,
                .h = cell_h
                });
        padding = cell_p;
    }
    anim.sheet = tex;
    anim.cells = cells;
    anim.num_cells = num_cells;
    anim.frame = 0;
    anim.rate = rate;
    
    anims.insert({anim_name, anim});
    if (anims.size() == 1) {
        currentAnim = anims.at(anim_name);
    }
}

void AnimatedSprite::play_animation(const char* anim_name, bool lp)
{
    if (anims.find(anim_name) == anims.end()) {
        fprintf(stderr, "Animation '%s' does not exist.", anim_name);
        return;
    }
    loop = lp;
    timer.start_timer();
    currentAnim = anims.at(anim_name);
    playing = true;
}

void AnimatedSprite::stop_animation()
{
    timer.stop_timer();
    playing = false;
}

void AnimatedSprite::update_animation()
{
    if (playing == false) {
        return;
    }
    if (timer.timer_get_ticks() > currentAnim.rate) {
        int nextFrame = (currentAnim.frame + 1) % currentAnim.num_cells;
        if (nextFrame == 0 && !loop) {
            stop_animation();
        } else {
            currentAnim.frame = nextFrame;
        }
        timer.start_timer();
    }
}

SDL_Rect AnimatedSprite::get_current_cell()
{
    return currentAnim.cells[currentAnim.frame];
}
