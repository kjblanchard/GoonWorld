#include <GoonWorld/gameobjects/GameObjects.hpp>
using namespace GoonWorld;
std::map<std::string, std::function<GameObject *(TiledMap::TiledObject &)>> GameSpawnMap = {
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
    {"Coin", [](TiledMap::TiledObject &object)
     {
         return new Coin(object);
     }},
    {"WinBox", [](TiledMap::TiledObject &object)
     {
         return new WinBox(object);
     }},
    {"ItemBox", [](TiledMap::TiledObject &object)
     {
         return new ItemBox(object);
     }},
    {"Brick", [](TiledMap::TiledObject &object)
     {
         return new ItemBrick(object);
     }}};