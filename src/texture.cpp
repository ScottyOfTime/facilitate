#include "texture.hpp"

Texture::Texture()
{
    tex = NULL;
    w = 0;
    h = 0;
    frame = 0;
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

void Texture::set_sheet(SDL_Rect *sh, int shSz)
{
    sheet = sh;
    sheetSz = shSz;
}

void Texture::render(int x, int y, SDL_Renderer *rend, SDL_Rect *clip)
{
    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    SDL_Rect renderQuad = {x, y, w, h};
    
    if (clip) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    if (SDL_RenderCopy(rend, tex, clip, &renderQuad) < 0) {
        printf("Can't render: %s\n", SDL_GetError());
    }
}

void Texture::render_sheet(int x, int y, SDL_Renderer *rend, float dt) 
{
    if (!timer.is_started()) {
        timer.start_timer();
    }
    if (timer.timer_get_ticks() > 100) {
        if (frame == sheetSz - 1) {
            frame = 0;
        } else {
            frame++;
        }
        timer.start_timer();
    }
    render(x, y, rend, &sheet[frame]);
}

uint8_t Texture::has_sheet()
{
    return sheet != NULL;
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
