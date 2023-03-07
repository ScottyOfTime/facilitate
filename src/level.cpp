/* Level class implementations */ 
#include "level.hpp"
#include "collision.hpp"
#include <fstream>
#include <iostream>

void Level::load_level_from_file(const char* path, Tilemap* tmap)
{
    std::fstream levelFile{path};
    std::string line;
    std::string delimiter = ",";
    int x = 0, y = 0;
    while (std::getline(levelFile, line)) {
        size_t pos = 0;
        std::string token;
        x = 0;
        while ((pos = line.find(delimiter)) != std::string::npos) {
            token = line.substr(0, pos);
            if (token.compare("floor")) {
                this->add_tile(x, y, 1, tmap, token);
            } else {
                this->add_tile(x, y, 0, tmap, token);
            }
            x += 56 * 2;
            line.erase(0, pos + delimiter.length());
        }
        y += 56 * 2;
    }
    levelFile.close();
}

void Level::add_tile(int x, int y, int tileType, Tilemap* tilemap, std::string tileName)
{
    if (tilemap->find_key(tileName)) {
        levelData.push_back(Tile(x, y, tileType, tilemap, tileName));
    }
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
