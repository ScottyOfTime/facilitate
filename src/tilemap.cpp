/* Loading and presenting a tilemap */
#include <iostream>
#include "tilemap.hpp"

/* Tile Class */
Tile::Tile(int x, int y, int tileType, Tilemap* tilemap, std::string tileName)
{
    pos.x = x;
    pos.y = y;

    tmap = tilemap; 
    std::cout << tileName << std::endl;
    clip = tmap->get_tile_clip(tileName);

    type = tileType;
}

int Tile::get_type()
{
    return type;
}

/* Returns clip in tilemap texture */
SDL_Rect* Tile::get_clip()
{
    return &clip;
}

Vec2 Tile::get_pos()
{
    return pos;
}

void Tile::render(SDL_Renderer *rend, SDL_Rect* camera)
{
    Texture* tex = tmap->get_texture();
    tex->render(pos.x - camera->x, pos.y - camera->y, rend, &clip, Vec2{2, 2});
}

/* Tilemap Class */
Tilemap::Tilemap(){}

uint8_t Tilemap::load_tilemap_from_file(const char* path, int tileWidth, int tileHeight, 
        SDL_Renderer *rend)
{
    tileW = tileWidth;
    tileH = tileHeight;
    return mapTex.load_from_file(path, rend);
}

void Tilemap::register_tile(std::string tileName, int x, int y)
{
    printf("%d, %d\n", tileW, tileH);
    SDL_Rect tileClip;
    tileClip.x = x;
    tileClip.y = y;
    tileClip.w = tileW;
    tileClip.h = tileH;
    tileClips.insert({tileName, tileClip});
}

SDL_Rect Tilemap::get_tile_clip(std::string tileName)
{
    SDL_Rect r = tileClips.at(tileName);
    return r;
}

uint8_t Tilemap::find_key(std::string key)
{
    if (tileClips.find(key) == tileClips.end())
        return 0;
    else
        return 1;
}

Texture* Tilemap::get_texture()
{
    return &mapTex;
}

void Tilemap::diagnose()
{
    fprintf(stderr, "Tilemap diagnostic:\n"
            "\tSize: %d\n"
            "\tFirst: %s\n", tileClips.size(), tileClips.begin()->first);
}
