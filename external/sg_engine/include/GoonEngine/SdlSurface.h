/**
 * @file SdlSurface.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-07-29
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <SDL2/SDL_types.h>
#include <SDL2/SDL_rect.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct SDL_Texture SDL_Texture;
    typedef struct SDL_Surface SDL_Surface;

    SDL_Surface *LoadSurfaceFromFile(const char *filePath, void* data);
    SDL_Surface *LoadTextureAtlas(int width, int height);
    // void BlitSurface(SDL_Surface *atlasSurface, SDL_Surface *tileSurface, SDL_Rect *dstRect, SDL_Rect *srcRect);
    void BlitSurface(SDL_Surface *srcSurface, SDL_Rect *srcRect, SDL_Surface *dstSurface, SDL_Rect *dstRect);
    SDL_Texture *CreateTextureFromSurface(SDL_Surface *surface);
    void DestroyPixelData(void* data);
    void DestroySurface(SDL_Surface* surface);
    void DestroyTexture(SDL_Texture* texture);
    void SetBackgroundAtlas(SDL_Texture *background, SDL_Rect *rect);
    SDL_Texture *CreateTextureFromFile(const char *filename);
    // void DrawTexture( SDL_Texture *texture, SDL_Rect *srcRect, SDL_Rect *dstRect);
    void DrawTexture(SDL_Texture *texture, SDL_Rect *srcRect, SDL_Rect *dstRect, bool shouldFlip);
    // int geDrawTexture( SDL_Texture * texture,
    //                    const SDL_Rect * srcrect,
    //                    const SDL_Rect * dstrect,
    //                    const double angle,
    //                    const SDL_Point *center,
    //                    const SDL_RendererFlip flip);
#ifdef __cplusplus
}
#endif