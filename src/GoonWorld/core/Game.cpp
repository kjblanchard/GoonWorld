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
#include <GoonWorld/ui/LogoPanel.hpp>
#include <GoonWorld/platformer/Helpers.hpp>
#include <GoonWorld/ui/BoxUi.hpp>
#include <GoonWorld/content/Text.hpp>
#include <GoonWorld/content/Image.hpp>

using namespace GoonWorld;

Game *Game::_gameInstance = nullptr;
Bgm *_loadingBgm = nullptr;
extern std::map<std::string, std::function<GameObject *(TiledMap::TiledObject &)>> GameSpawnMap;

Game::Game()
    : _loadingLevel(nullptr), _loadedLevel(nullptr), _deltaTime(0)
{
    if (_gameInstance)
    {
        LogCritical("Can only create one game instance");
    }
    _gameSettings = std::make_unique<AppSettings>("assets/config/appsettings.json");
    geInitializeRenderingWindow(_gameSettings->WindowConfig.WindowSize.x,
                                _gameSettings->WindowConfig.WindowSize.y,
                                _gameSettings->WindowConfig.WorldSize.x,
                                _gameSettings->WindowConfig.WorldSize.y,
                                _gameSettings->WindowConfig.Title.c_str());
    _sound = std::make_unique<Sound>(_gameSettings->SoundConfigs);
    _camera = std::make_unique<Camera>(geRectangle{0, 0, _gameSettings->WindowConfig.WorldSize.x, _gameSettings->WindowConfig.WorldSize.y});
    _gameInstance = this;
    InitializeLoadingLevel();

    // Platformer observers and event functions
    Helpers::AddMarioEventObserverFunctions();

    // _nextLevel = _gameSettings->DebugConfig.InitialLevel;
    if (!_gameSettings->MiscConfig.SkipLogos)
    {
        InitializeLogoLevel();
    }
    else
    {
        // ChangeToTiledLevel(_gameSettings->DebugConfig.InitialLevel);
        auto levelName = _gameSettings->DebugConfig.InitialLevel;
        _currentState = GameStates::Loading;
        _shouldChangeLevel = true;
        _nextLevel = levelName;
    }
    Content::LoadAllContent();
}

void Game::InitializeLogoLevel()
{
    _loadedLevel = std::make_unique<Level>();
    auto logoPanel = new LogoPanel();
    _loadedLevel->AddUiPanel(logoPanel);
    _currentState = GameStates::Logos;
}

void Game::InitializeLoadingLevel()
{
    _loadingLevel = std::make_unique<Level>();
    auto loadingPanel = new Panel();
    auto box = new BoxUi(geRectangle{0, 0, 512, 288});
    auto text = Text::TextFactory("World 1 - 1", Point{64, 20});
    auto texHeight = text->Height();
    auto mario = Image::ImageFactory("mario", geRectangle{64 + 20, texHeight + 5 + 40, 16, 40});
    auto livesLocation = Point{mario->Location().x + mario->Size().x + 20, mario->Location().y + 16};
    auto livesText = Text::TextFactory("x 99", livesLocation);
    mario->SetSrcRect(geRectangle{0, 0, 16, 40});
    loadingPanel->AddUIDrawObject(box);
    loadingPanel->AddText(text);
    loadingPanel->AddImage(mario);
    loadingPanel->AddText(livesText);
    _loadingBgm = Bgm::BgmFactory("loading");

    _loadingLevel->AddUiPanel(loadingPanel);
}

Game::~Game()
{
    _shouldChangeLevel = false;
    _shouldRestart = false;
    _tweens.clear();
    GameObject::ClearGameObjects();
    RigidbodyComponent::ResetRigidBodyVector();
    GameSpawnMap.clear();
    // TODO this should be ran here, however there is issues with deletion order due to Images/Text in UI and unique ptrs in panel
    // Content::ClearContent();
}

void Game::SetNextTiledLevelIfLevel()
{
    _nextLevel = _loadedLevel->GetTiledLevel().GetNextLevel();
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

    // Handle game paused and paused exceptions
    if (_paused)
    {
        for (auto pausedObject : _pauseUpdateObjects)
        {
            pausedObject->Update();
        }
        return;
    }

    // TODo this should be done differently.
    if (_switchNextFrame)
    {
        _currentLoadingTime = 0;
        _currentState = GameStates::Level;
        _switchNextFrame = _startedLoading = false;
        _loadedLevel->Start();
    }

    switch (_currentState)
    {
    case GameStates::Loading:
        if (!_startedLoading)
        {
            LoadLevel(_nextLevel);
            _startedLoading = true;
            _loadingBgm->Play(0);
        }
        _currentLoadingTime += totalSeconds;
        _loadingLevel->Update();
        if (_currentLoadingTime > _maxLoadingTime)
        {
            _switchNextFrame = true;
        }
        break;

    case GameStates::Level:
        if (_loadedLevel)
        {
            _loadedLevel->Update();
        }
        break;

    case GameStates::Logos:
        _loadedLevel->Update();
    default:
        break;
    }
}

void Game::Draw()
{
    switch (_currentState)
    {
    case GameStates::Loading:
        _loadingLevel->Draw();
        break;

    case GameStates::Level:
        if (_loadedLevel)
        {
            _loadedLevel->Draw();
        }
        break;

    case GameStates::Logos:
        _loadedLevel->Draw();
    default:
        break;
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

// This should restart a tiled level, AND NO LOADING SCREEN

void Game::RestartLevel()
{
    if (!_loadedLevel)
        return;
    _shouldChangeLevel = false;
    _shouldRestart = false;
    _currentState = GameStates::Loading;
    _loadingBgm->Play(0);
}

// Loads and starts a tiled level, this is called from the logo panel, and should just set the next level and change to loading screen
void Game::ChangeToTiledLevel(std::string &levelName)
{
    _currentState = GameStates::Loading;
    _shouldChangeLevel = true;
    _nextLevel = levelName;
}

// Loads a level, does not start.
void Game::LoadLevel(std::string level)
{
    // Cleanup
    _tweens.clear();
    _paused = false;
    _pauseUpdateObjects.clear();
    RigidbodyComponent::ResetRigidBodyVector();
    BoxColliderComponent::ResetBoxColliders();
    // Initialize
    if (_shouldChangeLevel)
    {
        _loadedLevel = std::make_unique<Level>(level.c_str());
        _shouldChangeLevel = false;
    }
    _loadedLevel->RestartLevel();
    Helpers::AddMarioUiToLevel(_loadedLevel.get());
    _camera->SetLevelSize(_loadedLevel->GetTiledLevel().GetSize());
    geSetCameraRect(_camera->Bounds());
    _camera->Restart();
    _loadedLevel->Load();
    Content::LoadAllContent();
    auto eventArgs = Event{this, this, (int)EventTypes::LevelStart};
    PushEvent(eventArgs);
}

// Happens during a frame update, loads and starts the next level, called at end of game level
void Game::ChangeLevel()
{
    _shouldRestart = false;
    _currentState = GameStates::Loading;
    LoadLevel(_nextLevel);
    _shouldChangeLevel = false;
    _loadingBgm->Play();
}

Level &Game::GetCurrentLevel()
{
    if (!_loadedLevel)
    {
        LogCritical("Getting a level when there isn't a loaded level will result in crash!");
    }
    return *_loadedLevel;
}
