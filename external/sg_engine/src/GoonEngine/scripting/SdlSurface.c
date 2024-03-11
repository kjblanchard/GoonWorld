#include <GoonEngine/gnpch.h>
#include <external/stb_image.h>
#include <GoonEngine/debug.h>
#include <GoonEngine/SdlSurface.h>

SDL_Texture *g_BackgroundAtlas = NULL;
SDL_Rect g_backgroundDrawRect;
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
    int width, height, channels;
    unsigned char *imageData = stbi_load(filePath, &width, &height, &channels, STBI_rgb_alpha);
    if (!imageData)
    {
        LogError("Failed to load image data: %s", stbi_failure_reason());
        return NULL;
    }
    // Calculate pitch
    int pitch;
    pitch = width * channels;
    pitch = (pitch + 3) & ~3;
    // Setup relevance bitmask
    int32_t Rmask, Gmask, Bmask, Amask;
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
    Rmask = 0x000000FF;
    Gmask = 0x0000FF00;
    Bmask = 0x00FF0000;
    Amask = (channels == 4) ? 0xFF000000 : 0;
#else
    int s = (bytesPerPixel == 4) ? 0 : 8;
    Rmask = 0xFF000000 >> s;
    Gmask = 0x00FF0000 >> s;
    Bmask = 0x0000FF00 >> s;
    Amask = 0x000000FF >> s;
#endif
    SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(imageData, width, height, channels * 8, pitch, Rmask, Gmask,
                                                    Bmask, Amask);

    if (!surface)
    {
        LogError("Could not create surface from data %s, Error:\n%s", filePath, SDL_GetError());
        return NULL;
    }
    stbi_image_free(imageData);
    return surface;
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
    if (result)
    {
        fprintf(stderr, "Failed to blit surface %s", SDL_GetError());
    }
}
SDL_Texture *CreateTextureFromFile(const char *filename)
{
    SDL_Surface *surface = LoadSurfaceFromFile(filename);
    SDL_Texture *texture = CreateTextureFromSurface(surface);
    return texture;
}

void DestroyTexture(SDL_Texture *texture)
{
    SDL_DestroyTexture(texture);
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

// void DrawTexture( SDL_Texture *texture, SDL_Rect *srcRect, SDL_Rect *dstRect)
void DrawTexture(SDL_Texture *texture, SDL_Rect *srcRect, SDL_Rect *dstRect, bool shouldFlip)
{
    // SDL_RenderCopy(g_pRenderer, texture, srcRect, dstRect);
    SDL_RenderCopyEx(g_pRenderer,
                     texture,
                     srcRect,
                     dstRect,
                     0,
                     NULL,
                     (shouldFlip) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}