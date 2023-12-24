#include <GoonEngine/gnpch.h>
#include <GoonEngine/SdlSurface.h>

SDL_Surface *LoadSurfaceFromFile(const char *filePath)
{
    SDL_Surface *tileSurface = IMG_Load(filePath);
    if (!tileSurface)
    {
        fprintf(stderr, "Could not load image %s, Error:\n%s", filePath, IMG_GetError());
        return NULL;
    }
    return tileSurface;
}
SDL_Surface *LoadTextureAtlas(int width, int height)
{
    SDL_Surface *atlasSurface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA8888);
    if (!atlasSurface)
    {
        fprintf(stderr, "Could not create atlast surface, Error: %s", SDL_GetError());
        return NULL;
    }
    return atlasSurface;
}

void BlitSurface(
    SDL_Surface *atlasSurface,
    SDL_Surface *tileSurface,
    SDL_Rect *dstRect,
    SDL_Rect *srcRect)
{
    SDL_BlitSurface(tileSurface, &srcRect, atlasSurface, &dstRect);
}

SDL_Texture *CreateTextureFromSurface(SDL_Renderer *renderer, SDL_Surface *surface)
{
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL)
    {
        fprintf(stderr, "Could not create texture, Error: %s", SDL_GetError());
        return NULL;
    }
    SDL_FreeSurface(surface); // We no longer need the surface after creating the texture
    return texture;
}

void DrawTexture(
    SDL_Renderer *renderer,
    SDL_Texture *texture,
    SDL_Rect *srcRect,
    SDL_Rect *dstRect )
{
    SDL_RenderCopy(renderer, texture, srcRect, dstRect);
}