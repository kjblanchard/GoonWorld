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
#include <GoonWorld/core/Level.hpp>
#include <GoonPhysics/GoonPhysics.h>
#include <GoonEngine/game.h>
#include <GoonEngine/color.h>
#include <GoonEngine/SdlSurface.h>
#include <GoonEngine/SdlWindow.h>
#include <GoonEngine/rectangle.h>
#include <GoonWorld/events/Observer.hpp>
#include <GoonWorld/events/EventTypes.hpp>

// Tweening test
#include <GoonWorld/interfaces/ITween.hpp>
#include <GoonWorld/tween/Tween.hpp>

// New Content test
#include <GoonWorld/content/Bgm.hpp>
#include <GoonWorld/content/Text.hpp>
#include <GoonWorld/ui/CoinsCollected.hpp>
#include <GoonWorld/ui/LevelTimer.hpp>

// Image test
// #include <GoonWorld/content/Image.hpp>
#include <GoonWorld/ui/LogoPanel.hpp>
// #include <GoonWorld/ui/Panel.hpp>
#include <GoonWorld/platformer/Helpers.hpp>
using namespace GoonWorld;

long long Game::_ticks = 0;
Game *Game::_gameInstance = nullptr;
extern std::map<std::string, std::function<GameObject *(TiledMap::TiledObject &)>> GameSpawnMap;

Game::Game()
    : _playerDying(nullptr), _playerBig(nullptr), _scene(nullptr), _loadingLevel(nullptr), _loadedLevel(nullptr), _deltaTime(0)
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

    // _coinUI = std::make_unique<CoinsCollectedUI>();
    // _levelTimerUI = std::make_unique<LevelTimer>();
    if (!_gameSettings->MiscConfig.SkipLogos)
    {
        _loadedLevel = std::make_unique<Level>();
        // TODO is this cleaned up somewhere in level?
        auto logoPanel = new LogoPanel();
        _loadedLevel->AddUpdateObject(logoPanel);
        _loadedLevel->AddDrawObject(logoPanel);
        _currentState = GameStates::Logos;
    }
    else
    {
        ChangeGameLevel(_gameSettings->DebugConfig.InitialLevel);
    }
    Content::LoadAllContent();
}

Game::~Game()
{
    _shouldChangeLevel = false;
    _shouldRestart = false;
    _playerDying = nullptr;
    _playerBig = nullptr;
    _tweens.clear();
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
    _camera->Update();
    auto deltaTimeSeconds = _deltaTime.GetTotalSeconds();
    for (auto &tween : _tweens)
    {
        if (!tween)
            continue;
        tween->Update(deltaTimeSeconds);
    }
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
    if (_loadedLevel)
    {
        _loadedLevel->Update();
    }
}

void Game::Draw()
{
    if (_loadedLevel)
    {
        _loadedLevel->Draw();
    }

    if (_gameSettings->DebugConfig.SolidDebug)
    {
        for (auto &solid : _loadedLevel->GetTiledLevel().GetAllSolidObjects())
        {
            auto box = geRectangle{solid.X, solid.Y, solid.Width, solid.Height};
            auto color = geColor{0, 255, 0, 255};
            geDrawDebugRect(&box, &color);
        }
    }

    // Move UI to it's own place?
    // for (auto object : UIDrawObjects)
    // {
    //     if (object->IsVisible())
    //         object->Draw();
    // }
}

void Game::ChangeGameLevel(std::string &levelName)
{
    _currentState = GameStates::Level;
    _shouldChangeLevel = true;
    LoadLevel(levelName);
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
    // _tweens.clear();
    if (!_loadedLevel)
        return;
    _shouldChangeLevel = false;
    _shouldRestart = false;
    _playerDying = nullptr;
    _playerBig = nullptr;
    if (_loadedLevel)
    {
        _loadedLevel->ClearObjects();
    }
    LoadLevel(_loadedLevel->GetTiledLevel().GetName());
    _loadedLevel->GetTiledLevel().RestartLevel();
}

void Game::LoadLevel(std::string level)
{
    // Cleanup
    _tweens.clear();
    GameObject::ClearGameObjects();
    RigidbodyComponent::ResetRigidBodyVector();
    BoxColliderComponent::ResetBoxColliders();
    // Initialize
    InitializePhysics();
    if (_shouldChangeLevel)
    {
        _loadedLevel = std::make_unique<Level>(level.c_str());
        _shouldChangeLevel = false;
    }
    auto gravity = _loadedLevel->GetTiledLevel().GetGravity();
    gpSceneSetGravity(_scene, gravity.y);
    gpSceneSetFriction(_scene, gravity.x);
    _loadedLevel->GetTiledLevel().SetTextureAtlas();
    _camera->SetLevelSize(_loadedLevel->GetTiledLevel().GetSize());
    geSetCameraRect(_camera->Bounds());
    auto [bgmName, bgmStart, bgmEnd, volume] = _loadedLevel->GetTiledLevel().GetBgmData();
    auto bgm = Bgm::BgmFactory(bgmName, bgmStart, bgmEnd);
    _camera->Restart();
    Helpers::AddMarioUiToLevel(_loadedLevel.get());
    LoadGameObjects();
    // _loadedLevel->GetTiledLevel().RestartLevel();
    Content::LoadAllContent();
    bgm->Play(-1, volume);
}

Panel *Game::CreateMarioLevelUi()
{
}

void Game::LoadGameObjects()
{
    for (auto &object : _loadedLevel->GetTiledLevel().GetAllObjects())
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
    if (_loadedLevel)
    {
        _loadedLevel->ClearObjects();
    }
    GameObject::ClearGameObjects();
    RigidbodyComponent::ResetRigidBodyVector();
    BoxColliderComponent::ResetBoxColliders();
    auto nextLevel = _loadedLevel->GetTiledLevel().GetNextLevel();
    LoadLevel(nextLevel);
    _shouldChangeLevel = false;
}

Level &Game::GetCurrentLevel()
{
    if (!_loadedLevel)
    {
        LogError("Getting a level when there isn't a loaded level will result in crash!");
    }
    return *_loadedLevel;
}
