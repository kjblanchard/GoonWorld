/**
 * @file SdlWindow.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-07-27
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
typedef struct SDL_Rect SDL_Rect;
typedef struct Color Color;

#ifdef __cplusplus
extern "C"
{
#endif

    int CreateWindowAndRenderer(unsigned int width, unsigned int height, const char *windowName);
    void DrawDebugRect(SDL_Rect *rect, Color *color);
#ifdef __cplusplus
}
#endif
