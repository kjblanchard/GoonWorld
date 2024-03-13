#include <GoonWorld/core/Game.hpp>
#include <GoonPhysics/GoonPhysics.h>
#include <GoonWorld/core/Sound.hpp>
#include <GoonEngine/color.h>
#include <GoonEngine/SdlWindow.h>
#include <GoonWorld/components/RigidbodyComponent.hpp>
#include <GoonWorld/base/GameObject.hpp>
#include <GoonWorld/interfaces/IUpdate.hpp>
#include <GoonWorld/interfaces/IDraw.hpp>
#include <GoonWorld/gameobjects/Player.hpp>
#include <GoonWorld/tiled/TiledLevel.hpp>
#include <GoonWorld/core/Content.hpp>
#include <GoonEngine/test.h>
#include <GoonPhysics/scene.h>
#include <GoonWorld/core/Camera.hpp>
#include <GoonEngine/SdlSurface.h>
using namespace GoonWorld;
Game *Game::_gameInstance = nullptr;
long long Game::_ticks = 0;
static const bool SOLID_DEBUG = false;

Game::Game(std::map<std::string, std::function<GameObject *(TiledMap::TiledObject &)>> spawnMap)
    : _spawnMap(spawnMap), _scene(nullptr), _playerDying(nullptr), _playerBig(nullptr), _loadedLevel(nullptr)
{
    if (_gameInstance)
    {
        fprintf(stderr, "Can only create one game instance");
        exit(1);
    }
    GameSettings = new AppSettings("assets/config/appsettings.json");
    CreateWindowAndRenderer(GameSettings->WindowConfig.WindowSize.x,
                            GameSettings->WindowConfig.WindowSize.y,
                            GameSettings->WindowConfig.Title.c_str());
    InitializePhysics();
    _sound = std::make_unique<Sound>(GameSettings->SoundConfigs);
    _camera = std::make_unique<Camera>(SDL_Rect{0, 0, GameSettings->WindowConfig.WindowSize.x, GameSettings->WindowConfig.WindowSize.y});
    _gameInstance = this;
}
Game::~Game()
{
    _spawnMap.clear();
    Content::ClearContent();
}
void Game::Update(double timeMs)
{
    ++_ticks;
    if (_shouldRestart)
        RestartLevel();
    if (!_playerBig)
    {
        RigidbodyComponent::PhysicsUpdate();
    }
    _camera->Update();
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

    if (_loadedLevel)
    {
        // Drawing debug rects on solids
        if (SOLID_DEBUG)
        {
            for (auto &solid : _loadedLevel->GetAllSolidObjects())
            {
                auto box = SDL_Rect{solid.X, solid.Y, solid.Width, solid.Height};
                auto color = Color{0, 255, 0, 255};
                DrawDebugRect(&box, &color);
            }
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
    if (!_shouldRestart)
    {
        _shouldRestart = true;
        return;
    }
    _shouldRestart = false;
    // for (size_t i = 0; i < 1000; i++)
    // {
    // Get rid of all update objects and draw objects
    UpdateObjects.clear();
    DrawObjects.clear();
    // Is there any more gameobjects we are forgetting about?
    // I think we should have a master list of gos
    GameObject::_gameobjects.clear();

    _playerDying = nullptr;
    _playerBig = nullptr;
    InitializePhysics();
    RigidbodyComponent::ResetRigidBodyVector();
    // LoadLevel(_loadedLevel->GetName());
    // Move this to func?

    auto result = _sound->LoadBgm("platforms");
    gpSceneSetGravity(_scene, _loadedLevel->GetGravity().y);
    gpSceneSetFriction(_scene, _loadedLevel->GetGravity().x);
    _loadedLevel->SetTextureAtlas();
    _sound->PlayBgm("platforms");
    _loadedLevel->RestartLevel();
    LoadGameObjects();
    _shouldRestart = false;
    _camera->Restart();
    // }
}
void Game::SetCurrentLevel(TiledLevel *level)
{
    _loadedLevel = std::unique_ptr<TiledLevel>(level);
}

void Game::LoadLevel(std::string level)
{

    auto result = _sound->LoadBgm("platforms");
    _loadedLevel = std::make_unique<TiledLevel>(level.c_str());
    gpSceneSetGravity(_scene, _loadedLevel->GetGravity().y);
    gpSceneSetFriction(_scene, _loadedLevel->GetGravity().x);
    _loadedLevel->SetTextureAtlas();
    _camera->SetLevelSize(_loadedLevel->GetSize());
    SetCameraRect(_camera->Bounds());
    _sound->PlayBgm("platforms");
    LoadGameObjects();
}
void Game::LoadGameObjects()
{
    for (auto &object : _loadedLevel->GetAllObjects())
    {
        auto iter = _spawnMap.find(object.ObjectType);
        if (iter == _spawnMap.end())
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