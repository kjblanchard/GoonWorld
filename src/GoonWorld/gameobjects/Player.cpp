#include <GoonWorld/gameobjects/Player.hpp>
#include <GoonWorld/components/DebugDrawComponent.hpp>

using namespace GoonWorld;

Player::Player(TiledMap::TiledObject &object)
{
    _location = Point{object.X, object.Y};
    _debugDrawComponent = new DebugDrawComponent(Point{object.Width, object.Height});
    AddComponent(_debugDrawComponent);
}
Player::~Player()
{
}