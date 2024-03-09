#include <GoonWorld/gameobjects/Player.hpp>
#include <GoonWorld/gameobjects/Goomba.hpp>
#include <GoonWorld/core/Game.hpp>
#include <GoonWorld/models/AppSettings.hpp>
#include <GoonWorld/core/Sound.hpp>
#include <GoonWorld/tiled/TiledLevel.hpp>
#include <GoonEngine/test.h>
#include <GoonEngine/SdlWindow.h>
#include <GoonPhysics/GoonPhysics.h>
#include <GoonEngine/SdlSurface.h>
#include <GoonEngine/color.h>
#include <GoonWorld/gameobjects/ItemBrick.hpp>
#include <GoonWorld/animation/Animator.hpp>
using namespace GoonWorld;

static Game *game;
static const bool SOLID_DEBUG = false;
static std::map<std::string, std::function<GameObject *(TiledMap::TiledObject &)>> GameObjectSpawnMap = {
    {"Player", [](TiledMap::TiledObject &object)
     {
         return new Player(object);
     }},
    {"Enemy", [](TiledMap::TiledObject &object)
     {
         return new Goomba(object);
     }},
    {"Brick", [](TiledMap::TiledObject &object)
     {
         return new ItemBrick(object);
     }}};
gpScene *InitializePhysics()
{
    auto scene = gpInitScene();
    geSetCurrentScene(scene);
    return scene;
}

void Update(double timeMs)
{
    game->Update(timeMs);
}
void Draw()
{
    game->Draw();
    if (game->GetCurrentLevel())
    {
        // Drawing debug rects on solids
        if (SOLID_DEBUG)
        {
            for (auto &solid : game->GetCurrentLevel()->GetAllSolidObjects())
            {
                auto box = SDL_Rect{solid.X, solid.Y, solid.Width, solid.Height};
                auto color = Color{0, 255, 0, 255};
                DrawDebugRect(&box, &color);
            }
        }
    }
}

int main()
{
    game = new Game();
    GnInitializeEngine();
    geGameSetUpdateFunc(Update);
    geGameSetDrawFunc(Draw);
    gsInitializeSound();
    game->GameSettings = new AppSettings("assets/config/appsettings.json");
    CreateWindowAndRenderer(game->GameSettings->WindowConfig.WindowSize.x,
                            game->GameSettings->WindowConfig.WindowSize.y,
                            game->GameSettings->WindowConfig.Title.c_str());
    auto scene = InitializePhysics();
    auto sound = new Sound(game->GameSettings->SoundConfigs);
    auto result = sound->LoadBgm("platformer");
    auto level1 = TiledLevel("level1");
    gpSceneSetGravity(scene, level1.GetGravity().y);
    gpSceneSetFriction(scene, level1.GetGravity().x);
    level1.SetTextureAtlas();
    game->SetCurrentLevel(&level1);
    sound->PlayBgm("platformer");
    for (auto &object : level1.GetAllObjects())
    {
        auto iter = GameObjectSpawnMap.find(object.ObjectType);
        if (iter == GameObjectSpawnMap.end())
            continue;
        (*iter).second(object);
    }

    Play();
}