#include <stdio.h>
#include <GoonEngine/test.h>
#include <GoonEngine/gnpch.h>
#include <GoonEngine/SdlWindow.h>
#include <SupergoonSound/include/sound.h>
// #include <GoonEngine/ecs/system.h>
#include <GoonEngine/ecs/context.h>
#include <GoonEngine/keyboard.h>

extern SDL_Texture *g_BackgroundAtlas;
extern SDL_Rect g_backgroundDrawRect;

#define MAX_STARTUP_FRAMES 1000

static SDL_Event event;
static bool shouldQuit = false;

static uint64_t lastFrameMilliseconds;
static float msBuildup;

// TODO this should be different, it is inside of SDLwindow.c
extern SDL_Renderer *g_pRenderer;
extern int g_refreshRate;
extern geContext* g_Context;



void *MusicUpdateWrapper(void *arg)
{
    return NULL;
}

/**
 * @brief Handles all SDL events every frame.
 *
 * @return true If we should quit or not
 * @return false If we should quit or not
 */
static bool sdlEventLoop()
{
    // Event loop, Handle SDL events.
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            return true;
            break;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            HandleKeyboardEvent(&event);
            break;
        default:
            break;
        }
    }
    return false;
}

static int loop_func()
{
    Uint64 beginFrame = SDL_GetTicks64();
    Uint64 delta = beginFrame - lastFrameMilliseconds;
    msBuildup += delta;
    lastFrameMilliseconds = beginFrame;
    // Handle SDL inputs
    shouldQuit = sdlEventLoop();
    if (shouldQuit)
        return 0;
    // TODO make these static and pass into as ref to stop allocations
    // Initialize time this frame
    double deltaTimeSeconds = 1 / (double)g_refreshRate;
    double deltaTimeMs = 1000 / (double)g_refreshRate;
    if (msBuildup < deltaTimeMs)
        return true;
    geUpdateKeyboard();

    // Run Update and update physics as many times as needed
    while (msBuildup >= deltaTimeMs)
    {
        UpdateSound();
        geContextUpdate(g_Context, NULL);
        msBuildup -= deltaTimeMs;
    }

    // Draw after we are caught up
    SDL_SetRenderDrawColor(g_pRenderer, 100, 100, 100, 255);
    SDL_RenderClear(g_pRenderer);
    if(g_BackgroundAtlas)
    {
        int drawResult = SDL_RenderCopy(g_pRenderer, g_BackgroundAtlas, &g_backgroundDrawRect, &g_backgroundDrawRect);

       if(drawResult != 0)
       {
            printf("Did not draw properly, Error %s\n", SDL_GetError());

       }
    }
    // SDL_SetRenderDrawColor(g_pRenderer, 255, 0, 0, 255);  // Set to red with full alpha
    // SDL_RenderDrawRect(g_pRenderer, &g_backgroundDrawRect);

    SDL_RenderPresent(g_pRenderer);
}

int Play()
{
    while (!shouldQuit)
    {
        loop_func();
    }
}
int GnInitializeEngine()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0)
    {
        fprintf(stderr, "Could not Initialize SDL!\nError: %s", SDL_GetError());
        return 1;
    }
    if(!IMG_Init(IMG_INIT_PNG))
    {
        fprintf(stderr, "Could not Initialize SDL Image!\nError: %s", IMG_GetError());
        return 1;
    }

    geInitializeKeyboard();

}
