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
typedef struct SDL_Surface SDL_Surface;

#include <SDL2/SDL_types.h>
#include <SDL2/SDL_rect.h>
typedef struct SDL_Texture SDL_Texture;

SDL_Surface *LoadSurfaceFromFile(const char *filePath);
SDL_Surface *LoadTextureAtlas(int width, int height);
// void BlitSurface(SDL_Surface *atlasSurface, SDL_Surface *tileSurface, SDL_Rect *dstRect, SDL_Rect *srcRect);
void BlitSurface( SDL_Surface *srcSurface, SDL_Rect *srcRect, SDL_Surface *dstSurface, SDL_Rect *dstRect);
SDL_Texture *CreateTextureFromSurface(SDL_Surface *surface);
void SetBackgroundAtlas(SDL_Texture *background, SDL_Rect *rect);
SDL_Texture *CreateTextureFromFile(const char* filename);
// void DrawTexture( SDL_Texture *texture, SDL_Rect *srcRect, SDL_Rect *dstRect);
void DrawTexture( SDL_Texture *texture, SDL_Rect *srcRect, SDL_Rect *dstRect, bool shouldFlip);
// int geDrawTexture( SDL_Texture * texture,
//                    const SDL_Rect * srcrect,
//                    const SDL_Rect * dstrect,
//                    const double angle,
//                    const SDL_Point *center,
//                    const SDL_RendererFlip flip);