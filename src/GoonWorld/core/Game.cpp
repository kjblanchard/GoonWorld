#include <GoonWorld/core/Game.hpp>
#include <GoonWorld/components/RigidbodyComponent.hpp>
#include <GoonWorld/base/GameObject.hpp>
#include <GoonWorld/interfaces/IUpdate.hpp>
#include <GoonWorld/interfaces/IDraw.hpp>
#include <GoonWorld/gameobjects/Player.hpp>
#include <GoonPhysics/scene.h>
using namespace GoonWorld;
Game *Game::_gameInstance = nullptr;
long long Game::_ticks = 0;

Game::Game()
{
    if (_gameInstance)
    {
        fprintf(stderr, "Can only create one game instance");
        exit(1);
    }
    _gameInstance = this;
}
Game::~Game()
{
}
void Game::Update(double timeMs)
{
    ++_ticks;
    if (!_playerBig)
    {
        RigidbodyComponent::PhysicsUpdate();
    }
    auto totalSeconds = timeMs / 1000;
    GameObject::DeltaTime = TimeSpan(totalSeconds);
    if (_playerDying || _playerBig)
    {
        if (_playerDying)
            _playerDying->Update();
        if (_playerBig)
            _playerBig->Update();
        return;
    }
    for (auto object : UpdateObjects)
    {
        object->Update();
    }
}
void Game::Draw()
{
    for (auto object : DrawObjects)
    {
        object->Draw();
    }
}

void Game::PlayerDie(Player *player)
{
    _playerDying = player;
}
void Game::PlayerBig(Player *player)
{
    _playerBig = player;
    if (_playerBig)
    {
        gpSceneSetEnabled(false);
    }
    else
    {
        gpSceneSetEnabled(true);
    }
}
