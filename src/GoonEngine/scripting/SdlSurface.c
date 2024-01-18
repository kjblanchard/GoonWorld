#include <GoonEngine/gnpch.h>
#include <GoonEngine/SdlSurface.h>

SDL_Texture *g_BackgroundAtlas = NULL;
SDL_Rect g_backgroundDrawRect = { 0, 0, 0, 0 };
extern SDL_Renderer *g_pRenderer;



void SetBackgroundAtlas(SDL_Texture *background, SDL_Rect *rect)
{
    g_BackgroundAtlas = background;
    g_backgroundDrawRect.x = rect->x;
    g_backgroundDrawRect.y = rect->y;
    g_backgroundDrawRect.h = rect->h;
    g_backgroundDrawRect.w = rect->w;
}

SDL_Surface *LoadSurfaceFromFile(const char *filePath)
{
    SDL_Surface *tileSurface = IMG_Load(filePath);
    if (!tileSurface)
    {
        fprintf(stderr, "Could not load image %s, Error:\n%s\n", filePath, IMG_GetError());
        return NULL;
    }
    return tileSurface;
}
SDL_Surface *LoadTextureAtlas(int width, int height)
{
    SDL_Surface *atlasSurface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA8888);
    // SDL_Surface *atlasSurface = SDL_CreateRGBSurface(0, width, height, 32, 0,0,0,0);
    if (!atlasSurface)
    {
        fprintf(stderr, "Could not create atlast surface, Error: %s", SDL_GetError());
        return NULL;
    }
    return atlasSurface;
}

void BlitSurface(
    SDL_Surface *srcSurface,
    SDL_Rect *srcRect,
    SDL_Surface *dstSurface,
    SDL_Rect *dstRect)
{
    int result = SDL_BlitSurface(srcSurface, srcRect, dstSurface, dstRect);
    if(result)
    {
        fprintf(stderr, "Failed to blit surface %s", SDL_GetError());
    }
}
SDL_Texture *CreateTextureFromFile(const char* filename)
{
    SDL_Surface* surface =  LoadSurfaceFromFile(filename);
    SDL_Texture* texture = CreateTextureFromSurface(surface);
    return texture;

}

SDL_Texture *CreateTextureFromSurface(SDL_Surface *surface)
{
    SDL_Texture *texture = SDL_CreateTextureFromSurface(g_pRenderer, surface);
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
    SDL_Rect *dstRect)
{
    SDL_RenderCopy(renderer, texture, srcRect, dstRect);
}