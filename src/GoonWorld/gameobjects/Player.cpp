#include <GoonWorld/gameobjects/Player.hpp>
#include <GoonWorld/components/DebugDrawComponent.hpp>

using namespace GoonWorld;

Player::Player()
{
    _debugDrawComponent = new DebugDrawComponent(Point{32, 32});
    AddComponent(_debugDrawComponent);
}
Player::~Player()
{
}