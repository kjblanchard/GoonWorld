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

SDL_Surface *LoadSurfaceFromFile(const char *filePath, void** data)
{
    int req_format = STBI_rgb_alpha;
int width, height, orig_format;
// unsigned char* data = stbi_load(filePath, &width, &height, &orig_format, req_format);
 *data = stbi_load(filePath, &width, &height, &orig_format, req_format);
if (*data == NULL) {
    SDL_Log("Loading image failed: %s", stbi_failure_reason());
    exit(1);
}

int depth, pitch;
Uint32 pixel_format;
if (req_format == STBI_rgb) {
    depth = 24;
    pitch = 3*width; // 3 bytes per pixel * pixels per row
    pixel_format = SDL_PIXELFORMAT_RGB24;
} else { // STBI_rgb_alpha (RGBA)
    depth = 32;
    pitch = 4*width;
    pixel_format = SDL_PIXELFORMAT_RGBA32;
}

SDL_Surface* surf = SDL_CreateRGBSurfaceWithFormatFrom((void*)*data, width, height,
                                                       depth, pitch, pixel_format);

if (surf == NULL) {
    SDL_Log("Creating surface failed: %s", SDL_GetError());
    stbi_image_free(*data);
    exit(1);
}

    if (!surf)
    {
        LogError("Could not create surface from data %s, Error:\n%s", filePath, SDL_GetError());
        return NULL;
    }
    // stbi_image_free(data);
    return surf;
}
    void DestroyPixelData(void* data)
    {
        stbi_image_free(data);

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
    void* data = NULL;
    SDL_Surface *surface = LoadSurfaceFromFile(filename, &data);
    SDL_Texture *texture = CreateTextureFromSurface(surface);
    stbi_image_free(data);
    return texture;
}
void DestroySurface(SDL_Surface *surface)
{
    SDL_FreeSurface(surface);
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