#include <iostream>
#include <GoonEngine/test.h>
#include <GoonEngine/SdlWindow.h>
#include <GoonEngine/Sound.h>
void Update(double deltaTime)
{
    printf("Update!\n");
}
void Draw()
{
    printf("Draw!\n");
}

int main()
{
    // Initialize engine
    GnInitializeEngine();
    geGameSetUpdateFunc(Update);
    geGameSetDrawFunc(Draw);
    CreateWindowAndRenderer(640, 480, "CPP TEST!");
    InitializeSound();
    auto bgmPtr = BgmLoad("assets/audio/test.ogg", 20.397, 43.08);
    BgmPlay(bgmPtr, 0.5);
    //
    Play();
}