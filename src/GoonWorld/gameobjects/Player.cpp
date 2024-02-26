#include <GoonWorld/gameobjects/Player.hpp>
#include <GoonWorld/components/DebugDrawComponent.hpp>
#include <GoonWorld/components/PlayerInputComponent.hpp>

using namespace GoonWorld;

Player::Player(TiledMap::TiledObject &object)
{
    _location = Point{object.X, object.Y};
    _debugDrawComponent = new DebugDrawComponent(Point{object.Width, object.Height});
    _playerInputComponent = new PlayerInputComponent(0);
    AddComponent({_debugDrawComponent, _playerInputComponent});
}
void Player::Update()
{
    // puts("Player update");
    if (_playerInputComponent->IsButtonDownOrHeld(GameControllerButton::DPAD_UP))
    {
        _location.y -= 1;
    }
    if (_playerInputComponent->IsButtonDownOrHeld(GameControllerButton::DPAD_DOWN))
    {
        _location.y += 1;
    }
    if (_playerInputComponent->IsButtonDownOrHeld(GameControllerButton::DPAD_LEFT))
    {
        _location.x -= 1;
    }
    if (_playerInputComponent->IsButtonDownOrHeld(GameControllerButton::DPAD_RIGHT))
    {
        _location.x += 1;
    }
}
Player::~Player()
{
}