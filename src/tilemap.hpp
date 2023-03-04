/* Class for loading and presenting tiles */
/* HELPFUL INFO:
 * The Tile class contains the tile type and the POSITION IN THE TEXTURE
 * where the tile is located, the actual render position is given in the 
 * render_tile function in the Tilemap class
 */
#include <map>
#include <string>
#include <vector>
#include "texture.hpp"

#define TILE_WIDTH 64
#define TILE_HEIGHT 64

#pragma once

// As they require each other pre-definitions are here
class Tile;
class Tilemap;

class Tile
{
public:
    Tile(int x, int y, int tileType, Tilemap* tilemap, std::string tileName);
    
    int get_type();
    Vec2 get_pos();
    SDL_Rect* get_clip();
    void render(SDL_Renderer *rend, SDL_Rect *camera);
    
private:
    Vec2 pos;
    SDL_Rect clip;
    Tilemap* tmap;
    int type;
};

class Tilemap
{
public:
    Tilemap();

    uint8_t load_tilemap_from_file(const char* path, int tileWidth, int tileHeight, 
            SDL_Renderer *rend);
    void register_tile(std::string tileName, int x, int y);
    SDL_Rect get_tile_clip(std::string tileName);
    uint8_t find_key(std::string key);

    Texture* get_texture();

    void diagnose();

private:
    int tileW, tileH;
    Texture mapTex;
    std::map<std::string, SDL_Rect> tileClips{};
};

