/* Level class */
#include <SDL.h>
#include <vector>
#include "tilemap.hpp"

#pragma once

class Level
{
public:
    void load_level_from_file(const char* path, Tilemap* tmap);
    /* 1 for collision 0 for none i think? */
    void add_tile(int x, int y, int tileType, Tilemap* tilemap, std::string tileName);
    void render_level(SDL_Renderer* rend, SDL_Rect* camera);
    uint8_t check_tile_collision(SDL_Rect *rect);
    void render_tile_collision(SDL_Renderer *rend, SDL_Rect *cam);
private:
    std::vector<Tile> levelData;
    int posX, posY;
};
