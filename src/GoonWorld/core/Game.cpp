#include <GoonWorld/core/Game.hpp>
#include <GoonWorld/components/RigidbodyComponent.hpp>
#include <GoonWorld/base/GameObject.hpp>
#include <GoonWorld/interfaces/IUpdate.hpp>
#include <GoonWorld/interfaces/IDraw.hpp>
using namespace GoonWorld;
Game *Game::_gameInstance = nullptr;

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
    RigidbodyComponent::PhysicsUpdate();
    auto totalSeconds = timeMs / 1000;
    GameObject::DeltaTime = TimeSpan(totalSeconds);
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