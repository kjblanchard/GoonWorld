#include <GoonWorld/gameobjects/Player.hpp>
#include <GoonWorld/components/DebugDrawComponent.hpp>
#include <GoonWorld/components/PlayerInputComponent.hpp>
#include <GoonWorld/components/RigidbodyComponent.hpp>
#include <SDL2/SDL_rect.h>

using namespace GoonWorld;

Player::Player(TiledMap::TiledObject &object)
{
    _location = Point{object.X, object.Y};
    _debugDrawComponent = new DebugDrawComponent(Point{object.Width, object.Height});
    _playerInputComponent = new PlayerInputComponent(0);
    auto bodyRect = SDL_Rect{object.X, object.Y, object.Width, object.Height};
    _rigidbodyComponent = new RigidbodyComponent(&bodyRect);
    AddComponent({_debugDrawComponent, _playerInputComponent, _rigidbodyComponent});
}
void Player::Update()
{
    // puts("Player update");
    if (_playerInputComponent->IsButtonDownOrHeld(GameControllerButton::DPAD_UP))
    {
        _rigidbodyComponent->Acceleration().y -= 15;
        // _location.y -= 1;
    }
    if (_playerInputComponent->IsButtonDownOrHeld(GameControllerButton::DPAD_DOWN))
    {
        _rigidbodyComponent->Acceleration().y += 15;
        // _location.y += 1;
    }
    if (_playerInputComponent->IsButtonDownOrHeld(GameControllerButton::DPAD_LEFT))
    {
        _rigidbodyComponent->Acceleration().x -= 15;
        // _location.x -= 1;
    }
    if (_playerInputComponent->IsButtonDownOrHeld(GameControllerButton::DPAD_RIGHT))
    {
        _rigidbodyComponent->Acceleration().x += 15;
        // _location.x += 1;
    }
}
Player::~Player()
{
}