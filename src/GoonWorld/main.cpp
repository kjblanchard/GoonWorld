#include <GoonWorld/gameobjects/Player.hpp>
#include <GoonWorld/core/Sound.hpp>
#include <GoonWorld/gameobjects/Goomba.hpp>
#include <GoonWorld/core/Game.hpp>
#include <GoonWorld/models/AppSettings.hpp>
#include <GoonEngine/test.h>
#include <GoonEngine/SdlWindow.h>
#include <GoonEngine/SdlSurface.h>
#include <GoonWorld/gameobjects/ItemBrick.hpp>
#include <GoonWorld/gameobjects/DeathBox.hpp>
#include <GoonWorld/animation/Animator.hpp>
using namespace GoonWorld;

static Game *game;
static std::map<std::string, std::function<GameObject *(TiledMap::TiledObject &)>> GameObjectSpawnMap = {
    {"Player", [](TiledMap::TiledObject &object)
     {
         return new Player(object);
     }},
    {"Enemy", [](TiledMap::TiledObject &object)
     {
         return new Goomba(object);
     }},
    {"DeathBox", [](TiledMap::TiledObject &object)
     {
         return new DeathBox(object);
     }},
    {"Brick", [](TiledMap::TiledObject &object)
     {
         return new ItemBrick(object);
     }}};

void Update(double timeMs)
{
    game->Update(timeMs);
}

void Draw()
{
    game->Draw();
}

int main()
{
    game = new Game(GameObjectSpawnMap);
    GnInitializeEngine();
    geGameSetUpdateFunc(Update);
    geGameSetDrawFunc(Draw);
    game->LoadLevel("level1");
    Play();
}