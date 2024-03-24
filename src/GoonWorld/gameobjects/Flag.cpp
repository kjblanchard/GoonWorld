#include <GoonWorld/gameobjects/Flag.hpp>
#include <GoonWorld/content/Sfx.hpp>
#include <GoonPhysics/overlap.h>
#include <GoonWorld/core/Content.hpp>
#include <GoonWorld/components/RigidbodyComponent.hpp>
#include <GoonWorld/components/DebugDrawComponent.hpp>
#include <GoonWorld/components/AnimationComponent.hpp>
#include <GoonWorld/events/Observer.hpp>
#include <GoonWorld/events/EventTypes.hpp>
#include <GoonWorld/core/Game.hpp>
using namespace GoonWorld;

Flag::Flag(TiledMap::TiledObject &object)
{
    _location = Point{object.X, object.Y};
    _previousLocation = Point{object.X, object.Y};
    auto bodyRect = geRectangle{object.X, object.Y, object.Width, object.Height};
    _rigidbodyComponent = new RigidbodyComponent(&bodyRect);
    _rigidbodyComponent->SetBodyType(BodyTypes::Flag);
    _animationComponent = new AnimationComponent("flags");
    AddComponent({_rigidbodyComponent, _animationComponent});

    _playerWinObserver = std::make_unique<Observer>((int)EventTypes::PlayerWin, [this](Event &event)
                                                    { this->PlayerWinEvent(event); });
    Game::Instance()->AddEventObserver((int)EventTypes::PlayerWin, _playerWinObserver.get());
    _rigidbodyComponent->GravityEnabled(false);
    _rigidbodyComponent->SetOverlapsEnabled(false);
}


void Flag::PlayerWinEvent(Event &event)
{
    _rigidbodyComponent->GravityEnabled(true);
    _rigidbodyComponent->SetOverlapsEnabled(true);
}

void Flag::TakeDamage()
{
    Enabled(false);
}
