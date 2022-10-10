/* Level class */
#include <SDL.h>
#include <vector>
#include "tilemap.hpp"

#pragma once

class Level
{
public:
    // load_level_from_file(const char* path)
    void add_tile(int x, int y, int tileType, Tilemap* tilemap, std::string tileName);
    void render_level(SDL_Renderer* rend, SDL_Rect* camera);
private:
    std::vector<Tile> levelData;
    int x, y;
};
