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

SDL_Surface *LoadSurfaceFromFile(const char *filePath);
SDL_Surface *LoadTextureAtlas(int width, int height);
void BlitSurface(SDL_Surface *atlasSurface, SDL_Surface *tileSurface, SDL_Rect *dstRect, SDL_Rect *srcRect);
SDL_Texture *CreateTextureFromSurface(SDL_Renderer *renderer, SDL_Surface *surface);
void DrawTexture(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *srcRect, SDL_Rect *dstRect);