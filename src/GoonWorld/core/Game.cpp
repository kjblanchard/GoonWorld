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
#include <GoonWorld/ui/Panel.hpp>
using namespace GoonWorld;

long long Game::_ticks = 0;
Game *Game::_gameInstance = nullptr;
extern std::map<std::string, std::function<GameObject *(TiledMap::TiledObject &)>> GameSpawnMap;

Game::Game()
    : DrawObjects(4), _playerDying(nullptr), _playerBig(nullptr), _scene(nullptr), _loadedLevel(nullptr), _deltaTime(0)
// : _playerDying(nullptr), _playerBig(nullptr), _scene(nullptr), _loadedLevel(nullptr), _deltaTime(0)
{
    // DrawObjects{4, {}};
    // std::vector<std::vector<IDraw*>> DrawObjects(4, std::vector<IDraw*>());
    // DrawObjects(4, std::vector<IDraw *>());

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
    logoPanel = std::make_unique<LogoPanel>();
    _currentState = GameStates::Logos;
    Content::LoadAllContent();
}

Game::~Game()
{
    _shouldChangeLevel = false;
    _shouldRestart = false;
    _playerDying = nullptr;
    _playerBig = nullptr;
    UpdateObjects.clear();
    DrawObjects.clear();
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
    if (_currentState == GameStates::Logos)
    {
        logoPanel->Update();
        auto deltaTimeSeconds = _deltaTime.GetTotalSeconds();
        for (auto &tween : _tweens)
        {
            if (!tween)
                continue;
            tween->Update(deltaTimeSeconds);
        }
    }
    else
    {

        if (_shouldRestart)
            RestartLevel();
        if (_shouldChangeLevel)
            ChangeLevel();
        // If there is not a player getting big, we should update physics.
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
        for (auto object : UpdateObjects)
        {
            auto updateEnable = dynamic_cast<IEnable *>(object);
            if (updateEnable && !updateEnable->IsEnabled())
                continue;
            object->Update();
        }
    }
}

void Game::Draw()
{
    if (_currentState == GameStates::Logos)
    {
        logoPanel->Draw();
    }
    else
    {
        for (auto layer : DrawObjects)
        {
            for (auto object : layer)
            {
                if (object->IsVisible())
                    object->Draw();
            }
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
                object->Draw();
        }
    }
}

void Game::StartGameLevel(std::string &levelName)
{
    _currentState = GameStates::Level;
    LoadLevel(levelName);
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
    _tweens.clear();
    if (!_loadedLevel)
        return;
    _shouldChangeLevel = false;
    _shouldRestart = false;
    _playerDying = nullptr;
    _playerBig = nullptr;
    UpdateObjects.clear();
    for (auto &layer : DrawObjects)
    {
        layer.clear();
    }
    UIDrawObjects.clear();
    GameObject::ClearGameObjects();
    RigidbodyComponent::ResetRigidBodyVector();
    BoxColliderComponent::ResetBoxColliders();
    LoadLevel(_loadedLevel->GetName());
    _loadedLevel->RestartLevel();
}

void Game::LoadLevel(std::string level)
{
    _tweens.clear();
    InitializePhysics();
    if (!_loadedLevel || _shouldChangeLevel)
    {
        _loadedLevel = std::make_unique<TiledLevel>(level.c_str());
    }
    gpSceneSetGravity(_scene, _loadedLevel->GetGravity().y);
    gpSceneSetFriction(_scene, _loadedLevel->GetGravity().x);
    _loadedLevel->SetTextureAtlas();
    _camera->SetLevelSize(_loadedLevel->GetSize());
    geSetCameraRect(_camera->Bounds());
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
    for (auto &layer : DrawObjects)
    {
        layer.clear();
    }
    GameObject::ClearGameObjects();
    RigidbodyComponent::ResetRigidBodyVector();
    BoxColliderComponent::ResetBoxColliders();
    auto nextLevel = _loadedLevel->GetNextLevel();
    LoadLevel(nextLevel);
    _shouldChangeLevel = false;
}

void Game::AddDrawObject(IDraw *draw)
{
    DrawObjects[draw->DrawLayer()].push_back(draw);
}

void Game::ChangeDrawObjectLayer(IDraw *draw, int newLayer)
{
    int currentLayer = draw->DrawLayer(); // Current layer of the IDraw object
    int objectIndex = -1;

    for (size_t i = 0; i < DrawObjects[currentLayer].size(); ++i)
    {

        if (DrawObjects[currentLayer][i] == draw)
        {
            objectIndex = i; // Store the current layer
            break;           // Exit the inner loop
        }
    }

    if (objectIndex != -1)
    {
        DrawObjects[newLayer].push_back(draw);
        DrawObjects[currentLayer].erase(DrawObjects[currentLayer].begin() + objectIndex);
    }
}