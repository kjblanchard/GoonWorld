#include <GoonEngine/gnpch.h>
#include <GoonEngine/test.h>
#include <GoonEngine/SdlWindow.h>
#include <GoonEngine/SdlSurface.h>
#include <GoonEngine/Sound.h>
#include <SDL2/SDL.h>
#include <GoonEngine/keyboard.h>
#include <GoonEngine/joystick.h>

#include <GoonPhysics/body.h>

void UpdateFunc()
{
    if (geGamepadButtonJustPressed(0, SDL_CONTROLLER_BUTTON_A))
    {
        printf("Just pressed button A\n");
    }
    if (geGamepadButtonJustReleased(0, SDL_CONTROLLER_BUTTON_A))
    {
        printf("Just released button A\n");
    }
    if (geGamepadButtonHeldDown(0, SDL_CONTROLLER_BUTTON_A))
    {
        printf("Holding button A\n");
    }

}

int main(int argc, char const *argv[])
{
    GnInitializeEngine();
    CreateWindowAndRenderer(300, 400, "Hello");
    SDL_Surface *purple = LoadSurfaceFromFile("assets/img/purple.png");
    SDL_Surface *atlas = LoadTextureAtlas(300, 400);
    SDL_Rect purpRect = {0, 0, 64, 64};
    SDL_Rect dstRect = {0, 0, 64, 64};
    BlitSurface(purple, &purpRect, atlas, &dstRect);
    SDL_Texture *tex = CreateTextureFromSurface(atlas);
    SDL_Rect screenRect = {0, 0, 64, 64};
    SetBackgroundAtlas(tex, &screenRect);
    struct Bgm *mainBgm = BgmLoad("test.ogg", 20.397, 43.08);
    BgmPlay(mainBgm, 1.0);
    geGameSetUpdateFunc(UpdateFunc);
    Play();
    return 0;
}
