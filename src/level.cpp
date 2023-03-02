/* Level class implementations */ 
#include "level.hpp"
#include "collision.hpp"
#include <fstream>
#include <iostream>

void Level::load_level_from_file(const char* path)
{
    std::fstream levelFile{path};
    std::string line;
    int x, y = 0;
    while (std::getline(levelFile, line)) {
        std::cout << line << std::endl; 
    }
}

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

uint8_t Level::check_tile_collision(SDL_Rect *rect)
{
    for (auto t : levelData) {
        if (t.get_type()) {
            Vec2 tilePos = t.get_pos();
            SDL_Rect *tileClip = t.get_clip();
            SDL_Rect tileRect{tilePos.x, tilePos.y, tileClip->w * 2, tileClip->h * 2};
            if (check_collision(&tileRect, rect)) {
                return 1;
            }
        }
    }
    return 0;
}

void Level::render_tile_collision(SDL_Renderer *rend, SDL_Rect *cam) 
{
    for (auto t : levelData) {
        if (t.get_type()) {
            Vec2 tilePos = t.get_pos();
            SDL_Rect *tileClip = t.get_clip();
            SDL_Rect tileRect{tilePos.x - cam->x, tilePos.y - cam->y, tileClip->w * 2, 
                tileClip->h * 2};
            SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
            SDL_RenderDrawRect(rend, &tileRect);
        }
    }
}
