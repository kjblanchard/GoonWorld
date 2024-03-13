#include <GoonWorld/core/Game.hpp>
#include <GoonWorld/core/Sound.hpp>
#include <GoonWorld/core/Content.hpp>
#include <GoonWorld/core/Camera.hpp>
#include <GoonWorld/components/RigidbodyComponent.hpp>
#include <GoonWorld/base/GameObject.hpp>
#include <GoonWorld/interfaces/IUpdate.hpp>
#include <GoonWorld/interfaces/IDraw.hpp>
#include <GoonWorld/gameobjects/Player.hpp>
#include <GoonWorld/tiled/TiledLevel.hpp>
#include <GoonPhysics/GoonPhysics.h>
#include <GoonEngine/test.h>
#include <GoonEngine/color.h>
#include <GoonEngine/SdlSurface.h>
#include <GoonEngine/SdlWindow.h>
#include <GoonEngine/rectangle.h>
using namespace GoonWorld;

long long Game::_ticks = 0;
Game *Game::_gameInstance = nullptr;
extern std::map<std::string, std::function<GameObject *(TiledMap::TiledObject &)>> GameSpawnMap;

Game::Game()
    : _scene(nullptr), _playerDying(nullptr), _playerBig(nullptr), _loadedLevel(nullptr)
{
    if (_gameInstance)
    {
        fprintf(stderr, "Can only create one game instance");
        exit(1);
    }
    GameSettings = new AppSettings("assets/config/appsettings.json");
    geInitializeRenderingWindow(GameSettings->WindowConfig.WindowSize.x,
                            GameSettings->WindowConfig.WindowSize.y,
                            GameSettings->WindowConfig.Title.c_str());
    _sound = std::make_unique<Sound>(GameSettings->SoundConfigs);
    _camera = std::make_unique<Camera>(geRectangle{0, 0, GameSettings->WindowConfig.WindowSize.x, GameSettings->WindowConfig.WindowSize.y});
    _gameInstance = this;
}

Game::~Game()
{
    GameSpawnMap.clear();
    Content::ClearContent();
}

void Game::Update(double timeMs)
{
    ++_ticks;
    auto totalSeconds = timeMs / 1000;
    GameObject::DeltaTime = TimeSpan(totalSeconds);

    if (_shouldRestart)
        RestartLevel();
    // If there is not a player getting big, we should update physics.
    if (!_playerBig)
    {
        RigidbodyComponent::PhysicsUpdate();
    }
    _camera->Update();

    // If there is a player dying or player getting big, we should only update them.
    if (_playerDying || _playerBig)
    {
        if (_playerDying)
            _playerDying->Update();
        if (_playerBig)
            _playerBig->Update();
        return;
    }

    GameObject::UpdateTimers();
    for (auto object : UpdateObjects)
    {
        object->Update();
    }
}

void Game::Draw()
{
    for (auto object : DrawObjects)
    {
        if (object->IsVisible())
            object->Draw();
    }

    if (GameSettings->DebugConfig.SolidDebug)
    {
        for (auto &solid : _loadedLevel->GetAllSolidObjects())
        {
            auto box = geRectangle{solid.X, solid.Y, solid.Width, solid.Height};
            auto color = geColor{0, 255, 0, 255};
            geDrawDebugRect(&box, &color);
        }
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

void Game::RestartLevel()
{
    if (!_loadedLevel)
        return;
    _shouldRestart = false;
    _playerDying = nullptr;
    _playerBig = nullptr;
    UpdateObjects.clear();
    DrawObjects.clear();
    GameObject::_gameobjects.clear();
    RigidbodyComponent::ResetRigidBodyVector();
    LoadLevel(_loadedLevel->GetName());
    _loadedLevel->RestartLevel();
}

void Game::SetCurrentLevel(TiledLevel *level)
{
    _loadedLevel = std::unique_ptr<TiledLevel>(level);
}

void Game::LoadLevel(std::string level)
{
    InitializePhysics();
    auto result = _sound->LoadBgm("platforms");
    if (!_loadedLevel)
    {
        _loadedLevel = std::make_unique<TiledLevel>(level.c_str());
    }
    gpSceneSetGravity(_scene, _loadedLevel->GetGravity().y);
    gpSceneSetFriction(_scene, _loadedLevel->GetGravity().x);
    _loadedLevel->SetTextureAtlas();
    _camera->SetLevelSize(_loadedLevel->GetSize());
    SetCameraRect(_camera->Bounds());
    _sound->PlayBgm("platforms");
    _camera->Restart();
    LoadGameObjects();
}
void Game::LoadGameObjects()
{
    for (auto &object : _loadedLevel->GetAllObjects())
    {
        auto iter = GameSpawnMap.find(object.ObjectType);
        if (iter == GameSpawnMap.end())
            continue;
        (*iter).second(object);
    }
}
void Game::InitializePhysics()
{
    if (_scene)
        gpSceneFree(_scene);
    _scene = gpInitScene();
    geSetCurrentScene(_scene);
}
