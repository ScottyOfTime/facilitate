#include "texture.hpp"

Texture::Texture()
{
    tex = NULL;
    w = 0;
    h = 0;
}

Texture::~Texture()
{
    free();
}

uint8_t Texture::load_from_file(const char *path, SDL_Renderer *rend)
{
    free();

    SDL_Texture *newTex = NULL;

    SDL_Surface *surf = IMG_Load(path);
    if (surf == NULL) {
        fprintf(stderr, "Unable to load image %s, SDL_Image Error: %s",
                path, IMG_GetError());
    } else {
        //SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf->format, 0x0, 0x0, 0x0));

        newTex = SDL_CreateTextureFromSurface(rend, surf);
        if (newTex == NULL) {
            printf("Unable to create texture from %s, SDL Error: %s",
                    path, SDL_GetError());
        } else {
            w = surf->w;
            h = surf->h;
        }
        SDL_FreeSurface(surf);
    }
    tex = newTex;
    return tex != NULL;
}


void Texture::render(int x, int y, SDL_Renderer *rend, SDL_Rect *clip, Vec2 scale)
{
    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    SDL_Rect renderQuad = {x, y, w , h};
    
    if (clip) {
        renderQuad.w = clip->w * scale.x;
        renderQuad.h = clip->h * scale.y;
    }

    if (SDL_RenderCopy(rend, tex, clip, &renderQuad) < 0) {
        printf("Can't render: %s\n", SDL_GetError());
    }
}

void Texture::free()
{
    if (tex != NULL) {
        SDL_DestroyTexture(tex);
        tex = NULL;
        w = 0;
        h = 0;
    }
}
