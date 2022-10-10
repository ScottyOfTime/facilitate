/* Loading and presenting a tilemap */
#include <iostream>
#include "tilemap.hpp"

/* Tile Class */
Tile::Tile(int x, int y, int tileType, Tilemap* tilemap, std::string tileName)
{
    posX = x;
    posY = y;

    tmap = tilemap; 
    clip = tmap->get_tile_clip(tileName);

    type = tileType;
}

SDL_Rect* Tile::get_clip()
{
    return &clip;
}

void Tile::render(SDL_Renderer *rend, SDL_Rect* camera)
{
    Texture* tex = tmap->get_texture();
    tex->render(posX - camera->x, posY - camera->y, rend, &clip);
}

/* Tilemap Class */
Tilemap::Tilemap(){}

uint8_t Tilemap::load_tilemap_from_file(const char* path, int tileWidth, int tileHeight, 
        SDL_Renderer *rend)
{
    return mapTex.load_from_file(path, rend);
}

void Tilemap::register_tile(std::string tileName, int x, int y)
{
    SDL_Rect tileClip;
    tileClip.x = x;
    tileClip.y = y;
    tileClip.w = TILE_WIDTH;
    tileClip.h = TILE_HEIGHT;
    tileClips.insert({tileName, tileClip});
}

SDL_Rect Tilemap::get_tile_clip(std::string tileName)
{
    SDL_Rect r = tileClips.at(tileName);
    return r;
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
