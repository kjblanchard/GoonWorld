#pragma once
#include <vector>
#include <unordered_map>
#include <queue>
#include <GoonWorld/base/GameObject.hpp>
#include <GoonWorld/events/Event.hpp>
#include <GoonWorld/interfaces/ITween.hpp>

typedef struct gpScene gpScene;

namespace GoonWorld
{
    enum class GameStates
    {
        Default,
        Logos,
        Level,
        Loading,
    };
    class IUpdate;
    class IDraw;
    struct Observer;
    class AppSettings;
    // class TiledLevel;
    class Level;
    class Player;
    class Sound;
    class Camera;
    class CoinsCollectedUI;
    class LevelTimer;
    class Image;
    class Panel;
    class LogoPanel;

    class Game
    {
    public:
        static inline Game *Instance() { return _gameInstance; }
        inline long long GetTicks() { return _ticks; }
        Game();
        ~Game();
        void Update(double time);
        void Draw();
        inline void AddTween(ITween *tween) { _tweens.push_back(std::unique_ptr<ITween>(tween)); }
        inline void TriggerRestartLevel() { _shouldRestart = true; }
        inline void TriggerNextLevel() { _shouldChangeLevel = true; }
        inline Sound *GetSound() const { return _sound.get(); }
        inline Camera *GetCamera() { return _camera.get(); }
        inline void AddEventObserver(int event, Observer *observer) { _observers[event].push_back(observer); }
        inline AppSettings &GetAppSettings() { return *_gameSettings; }
        inline TimeSpan DeltaTime() { return _deltaTime; }
        inline void AddPauseUpdateObject(IUpdate *obj) { _pauseUpdateObjects.push_back(obj); }
        inline void RemovePauseUpdateObject(IUpdate *obj) { _pauseUpdateObjects.erase(std::remove(_pauseUpdateObjects.begin(), _pauseUpdateObjects.end(), obj), _pauseUpdateObjects.end()); }
        inline void PauseGame(bool isPaused) { _paused = isPaused; }
        Level &GetCurrentLevel();
        void ChangeToTiledLevel(std::string &levelName);
        void RemoveObserver(Observer *observer);
        void PushEvent(Event event);
        void LoadLevel(std::string levelName);

    private:
        std::vector<std::unique_ptr<ITween>> _tweens;

        // Paused Stuff
        bool _paused;
        std::vector<IUpdate *> _pauseUpdateObjects;

        void InitializeLogoLevel();
        void LoadGameObjects();
        void InitializePhysics();
        void RestartLevel();
        void ChangeLevel();
        bool _shouldRestart = false;
        bool _shouldChangeLevel = false;
        gpScene *_scene;
        static Game *_gameInstance;
        long long _ticks = 0;
        std::unordered_map<int, std::vector<Observer *>> _observers;
        std::unique_ptr<Level> _loadingLevel;
        std::unique_ptr<Level> _loadedLevel;
        std::unique_ptr<Sound> _sound;
        std::unique_ptr<Camera> _camera;
        std::unique_ptr<AppSettings> _gameSettings;

        GameStates _currentState = GameStates::Default;
        TimeSpan _deltaTime;
    };
}