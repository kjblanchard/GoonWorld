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

    class Game
    {
    public:
        static inline Game *Instance() { return _gameInstance; }
        static inline long long GetTicks() { return _ticks; }
        std::vector<std::shared_ptr<IUpdate>> UpdateObjects;
        std::vector<IDraw *> DrawObjects;
        inline void SetCurrentLevel(TiledLevel *level) { _loadedLevel = level; }
        inline TiledLevel *GetCurrentLevel() const { return _loadedLevel; }
        inline Sound *GetSound() const { return _sound; }
        inline void SetSound(Sound *sound)
        {
            if (!_sound)
                _sound = sound;
        }
        void PlayerDie(Player *player);
        void PlayerBig(Player *player);
        AppSettings *GameSettings;
        void RestartLevel();
        void LoadLevel(std::string levelName);

        Game(std::map<std::string, std::function<GameObject *(TiledMap::TiledObject &)>> spawnMap);
        ~Game();
        void Update(double time);
        void Draw();

    private:
        void LoadGameObjects();
        void InitializePhysics();
        Player *_playerDying;
        Player *_playerBig;
        bool _shouldRestart = false;
        gpScene* _scene;
        // TiledLevel* _currentLevel;
        static Game *_gameInstance;
        static long long _ticks;
        TiledLevel *_loadedLevel;
        Sound *_sound;
        std::map<std::string, std::function<GameObject *(TiledMap::TiledObject &)>> _spawnMap;
    };
}