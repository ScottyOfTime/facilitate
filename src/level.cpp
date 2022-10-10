/* Level class implementations */ 
#include "level.hpp"

void Level::add_tile(int x, int y, int tileType, Tilemap* tilemap, std::string tileName)
{
    levelData.push_back(Tile(x, y, tileType, tilemap, tileName));
}

void Level::render_level(SDL_Renderer* rend, SDL_Rect* camera)
{
    for (auto t : levelData) {
       t.render(rend, camera);
    }
}
