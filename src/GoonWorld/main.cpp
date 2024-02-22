#include <iostream>
#include <algorithm>
#include <GoonWorld/models/AppSettings.hpp>
#include <GoonWorld/core/Sound.hpp>
#include <GoonWorld/tiled/TiledLevel.hpp>
#include <GoonEngine/test.h>
#include <GoonEngine/SdlWindow.h>
#include <GoonPhysics/GoonPhysics.h>
using namespace GoonWorld;

void Update(double deltaTime)
{
}

void Draw()
{
}

void InitializePhysics()
{
    auto scene = gpInitScene();
    geSetCurrentScene(scene);
    gpSceneSetGravity(scene, 50);
}

int main()
{
    GnInitializeEngine();
    geGameSetUpdateFunc(Update);
    geGameSetDrawFunc(Draw);
    InitializeSound();
    auto settings = new AppSettings("assets/config/appsettings.json");
    CreateWindowAndRenderer(settings->WindowConfig.WindowSize.x,
                            settings->WindowConfig.WindowSize.y,
                            settings->WindowConfig.Title.c_str());
    InitializePhysics();
    auto sound = new Sound(settings->SoundConfigs);
    auto result = sound->LoadBgm("rangers");
    auto level1 = TiledLevel("level1");
    level1.SetTextureAtlas();
    sound->PlayBgm("rangers");
    Play();
}