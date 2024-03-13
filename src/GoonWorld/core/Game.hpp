#pragma once
#include <vector>
#include <GoonWorld/base/GameObject.hpp>

typedef struct gpScene gpScene;

namespace GoonWorld
{
    class IUpdate;
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
        inline TiledLevel *GetCurrentLevel() const { return _loadedLevel.get(); }
        inline Sound *GetSound() const { return _sound.get(); }
        inline Camera *GetCamera() { return _camera.get(); }
        inline void AddUpdateObject(IUpdate* update) { UpdateObjects.push_back(update); }
        inline void AddDrawObject(IDraw* draw) { DrawObjects.push_back(draw); }
        void SetCurrentLevel(TiledLevel *level);
        // Player Dying or getting big functions, maybe this should be an event?
        void PlayerDie(Player *player);
        void PlayerBig(Player *player);
        AppSettings *GameSettings;
        void LoadLevel(std::string levelName);

    private:
        std::vector<IUpdate*> UpdateObjects;
        std::vector<IDraw*> DrawObjects;
        void LoadGameObjects();
        void InitializePhysics();
        void RestartLevel();
        Player *_playerDying;
        Player *_playerBig;
        bool _shouldRestart = false;
        gpScene *_scene;
        // TiledLevel* _currentLevel;
        static Game *_gameInstance;
        static long long _ticks;
        std::unique_ptr<TiledLevel> _loadedLevel;
        std::unique_ptr<Sound> _sound;
        std::unique_ptr<Camera> _camera;
    };
}