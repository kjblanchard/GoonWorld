#pragma once
#include <vector>

namespace GoonWorld
{
    class IUpdate;
    class IDraw;
    class AppSettings;

    class Game
    {
    public:
        static inline Game *Instance() { return _gameInstance; }
        std::vector<std::shared_ptr<IUpdate> > UpdateObjects;
        std::vector<IDraw *> DrawObjects;
        AppSettings* GameSettings;

        Game();
        ~Game();
        void Update();
        void Draw();

    private:
        static Game *_gameInstance;
    };
}