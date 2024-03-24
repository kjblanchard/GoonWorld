#include <GoonWorld/core/Game.hpp>
#include <GoonWorld/core/Sound.hpp>
#include <GoonWorld/core/Content.hpp>
#include <GoonWorld/core/Camera.hpp>
#include <GoonWorld/components/RigidbodyComponent.hpp>
#include <GoonWorld/components/BoxColliderComponent.hpp>
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
#include <GoonWorld/events/Observer.hpp>
#include <GoonWorld/events/EventTypes.hpp>

#include <GoonWorld/content/Bgm.hpp>
#include <GoonWorld/content/Text.hpp>
#include <GoonWorld/ui/CoinsCollected.hpp>
#include <GoonWorld/ui/LevelTimer.hpp>
using namespace GoonWorld;

long long Game::_ticks = 0;
Game *Game::_gameInstance = nullptr;
extern std::map<std::string, std::function<GameObject *(TiledMap::TiledObject &)>> GameSpawnMap;

Game::Game()
    : _scene(nullptr), _playerDying(nullptr), _playerBig(nullptr), _loadedLevel(nullptr), _deltaTime(0)
{
    if (_gameInstance)
    {
        fprintf(stderr, "Can only create one game instance");
        exit(1);
    }
    _gameSettings = std::make_unique<AppSettings>("assets/config/appsettings.json");
    geInitializeRenderingWindow(_gameSettings->WindowConfig.WindowSize.x,
                                _gameSettings->WindowConfig.WindowSize.y,
                                _gameSettings->WindowConfig.WorldSize.x,
                                _gameSettings->WindowConfig.WorldSize.y,
                                _gameSettings->WindowConfig.Title.c_str());
    _playerBigObserver = std::make_unique<Observer>((int)EventTypes::PlayerBig, [this](Event &event)
                                                    { this->PlayerBigEvent(event); });
    _playerDieObserver = std::make_unique<Observer>((int)EventTypes::PlayerDie, [this](Event &event)
                                                    { this->PlayerDieEvent(event); });
    AddEventObserver((int)EventTypes::PlayerBig, _playerBigObserver.get());
    AddEventObserver((int)EventTypes::PlayerDie, _playerDieObserver.get());
    _sound = std::make_unique<Sound>(_gameSettings->SoundConfigs);
    _camera = std::make_unique<Camera>(geRectangle{0, 0, _gameSettings->WindowConfig.WorldSize.x, _gameSettings->WindowConfig.WorldSize.y});
    _gameInstance = this;
    _coinUI = std::make_unique<CoinsCollectedUI>();
    _levelTimerUI = std::make_unique<LevelTimer>();
}

Game::~Game()
{
    _shouldChangeLevel = false;
    _shouldRestart = false;
    _playerDying = nullptr;
    _playerBig = nullptr;
    UpdateObjects.clear();
    DrawObjects.clear();
    GameObject::ClearGameObjects();
    RigidbodyComponent::ResetRigidBodyVector();
    GameSpawnMap.clear();
    Content::ClearContent();
}

void Game::Update(double timeMs)
{
    ++_ticks;
    auto totalSeconds = timeMs / 1000;
    GameObject::DeltaTime = TimeSpan(totalSeconds);
    _deltaTime = TimeSpan(totalSeconds);

    if (_shouldRestart)
        RestartLevel();
    if (_shouldChangeLevel)
        ChangeLevel();
    // If there is not a player getting big, we should update physics.

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
        auto updateEnable = dynamic_cast<IEnable *>(object);
        if (updateEnable && !updateEnable->IsEnabled())
            continue;
        object->Update();
    }
}
void Game::CameraUpdate(double accum)
{
    _camera->Update(accum);
}

void Game::Draw(double accum)
{

    for (auto object : DrawObjects)
    {
        if (object->IsVisible())
            object->Draw(accum);
    }

    if (_gameSettings->DebugConfig.SolidDebug)
    {
        for (auto &solid : _loadedLevel->GetAllSolidObjects())
        {
            auto box = geRectangle{solid.X, solid.Y, solid.Width, solid.Height};
            auto color = geColor{0, 255, 0, 255};
            geDrawDebugRect(&box, &color);
        }
    }

    for (auto object : UIDrawObjects)
    {
        if (object->IsVisible())
            object->Draw(accum);
    }
}

void Game::SetCurrentLevel(TiledLevel *level)
{
    _loadedLevel = std::unique_ptr<TiledLevel>(level);
}

void Game::RemoveObserver(Observer *observer)
{
    auto &vec = _observers.at(observer->EventType);
    for (size_t i = 0; i < vec.size(); i++)
    {
        // if (vec[i] == observer)
        // {
        //     // vec[i] = nullptr;
        //     return;
        // }
        for (auto it = vec.begin(); it != vec.end(); ++it)
        {
            if (*it == observer)
            {
                vec.erase(it);
                return;
            }
        }
    }
}

void Game::PushEvent(Event event)
{
    auto observersVector = _observers[event.eventType];
    for (auto observer : observersVector)
    {
        // TODO should clean the nullptrs out of this list one day
        if (!observer)
            continue;
        observer->Function(event);
    }
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
    _shouldChangeLevel = false;
    _shouldRestart = false;
    _playerDying = nullptr;
    _playerBig = nullptr;
    UpdateObjects.clear();
    DrawObjects.clear();
    UIDrawObjects.clear();
    GameObject::ClearGameObjects();
    RigidbodyComponent::ResetRigidBodyVector();
    BoxColliderComponent::ResetBoxColliders();
    LoadLevel(_loadedLevel->GetName());
    _loadedLevel->RestartLevel();
}

void Game::LoadLevel(std::string level)
{
    InitializePhysics();
    if (!_loadedLevel || _shouldChangeLevel)
    {
        _loadedLevel = std::make_unique<TiledLevel>(level.c_str());
    }
    gpSceneSetGravity(_scene, _loadedLevel->GetGravity().y);
    gpSceneSetFriction(_scene, _loadedLevel->GetGravity().x);
    _loadedLevel->SetTextureAtlas();
    _camera->SetLevelSize(_loadedLevel->GetSize());
    SetCameraRect(_camera->Bounds());
    auto bgm = Bgm::BgmFactory(_loadedLevel->BgmName().c_str(), _loadedLevel->BgmLoopStart(), _loadedLevel->BgmLoopEnd());
    _camera->Restart();
    LoadGameObjects();
    Content::LoadAllContent();
    AddUIObject(_coinUI.get());
    AddUIObject(_levelTimerUI.get());
    AddUpdateObject(_levelTimerUI.get());
    bgm->Play(-1, _loadedLevel->BgmVolume());
    _coinUI->UpdateCoins(0);
    _levelTimerUI->UpdateTime(0);
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

void Game::PlayerBigEvent(Event &event)
{
    auto player = static_cast<Player *>(event.eventArgs);
    PlayerBig(player);
}

void Game::PlayerDieEvent(Event &event)
{
    auto player = static_cast<Player *>(event.eventArgs);
    PlayerDie(player);
}

void Game::ChangeLevel()
{
    _shouldRestart = false;
    _playerDying = nullptr;
    _playerBig = nullptr;
    UpdateObjects.clear();
    DrawObjects.clear();
    GameObject::ClearGameObjects();
    RigidbodyComponent::ResetRigidBodyVector();
    BoxColliderComponent::ResetBoxColliders();
    auto nextLevel = _loadedLevel->GetNextLevel();
    LoadLevel(nextLevel);
    _shouldChangeLevel = false;
}