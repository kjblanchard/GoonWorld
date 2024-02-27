#pragma once
#include <vector>

namespace GoonWorld
{
    class IUpdate;
    class IDraw;
    class AppSettings;
    class TiledLevel;

    class Game
    {
    public:
        static inline Game *Instance() { return _gameInstance; }
        std::vector<std::shared_ptr<IUpdate> > UpdateObjects;
        std::vector<IDraw *> DrawObjects;
        inline void SetCurrentLevel(TiledLevel* level) { _loadedLevel = level; }
        inline TiledLevel* GetCurrentLevel() const { return _loadedLevel;}
        AppSettings* GameSettings;

        Game();
        ~Game();
        void Update(double time);
        void Draw();

    private:
        static Game *_gameInstance;
        TiledLevel* _loadedLevel;

    };
}