#pragma once
#include <vector>
#include <queue>
#include <GoonWorld/base/GameObject.hpp>
#include <GoonWorld/events/Event.hpp>

typedef struct gpScene gpScene;

namespace GoonWorld
{
    class IUpdate;
    class Observer;
    class IDraw;
    class AppSettings;
    class TiledLevel;
    class Player;
    class Sound;
    class Camera;

    class Game
    {
    public:
        static inline Game *Instance() { return _gameInstance; }
        static inline long long GetTicks() { return _ticks; }
        Game();
        ~Game();
        void Update(double time);
        void Draw();
        inline void TriggerRestartLevel() { _shouldRestart = true; }
        inline void TriggerNextLevel() { _shouldChangeLevel = true; }
        inline TiledLevel *GetCurrentLevel() const { return _loadedLevel.get(); }
        inline Sound *GetSound() const { return _sound.get(); }
        inline Camera *GetCamera() { return _camera.get(); }
        inline void AddEventObserver(int event, Observer *observer) { _observers[event].push_back(observer); }
        inline void AddUpdateObject(IUpdate *update) { UpdateObjects.push_back(update); }
        inline void AddDrawObject(IDraw *draw) { DrawObjects.push_back(draw); }
        inline AppSettings &GetAppSettings() { return *_gameSettings; }
        void SetCurrentLevel(TiledLevel *level);
        void RemoveObserver(Observer *observer);
        void PushEvent(Event event);
        void LoadLevel(std::string levelName);

    private:
        inline void PlayerDie(Player *player) { _playerDying = player; }
        void PlayerBig(Player *player);
        std::vector<IUpdate *> UpdateObjects;
        std::vector<IDraw *> DrawObjects;
        void PlayerBigEvent(Event &event);
        void PlayerDieEvent(Event &event);
        void LoadGameObjects();
        void InitializePhysics();
        void RestartLevel();
        void ChangeLevel();
        Player *_playerDying;
        Player *_playerBig;
        bool _shouldRestart = false;
        bool _shouldChangeLevel = false;
        gpScene *_scene;
        static Game *_gameInstance;
        static long long _ticks;
        std::unordered_map<int, std::vector<Observer *>> _observers;
        std::unique_ptr<TiledLevel> _loadedLevel;
        std::unique_ptr<Sound> _sound;
        std::unique_ptr<Camera> _camera;
        std::unique_ptr<Observer> _playerBigObserver;
        std::unique_ptr<Observer> _playerDieObserver;
        std::unique_ptr<AppSettings> _gameSettings;
    };
}