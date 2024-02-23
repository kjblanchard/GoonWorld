#pragma once
#include <vector>

namespace GoonWorld
{
    class IUpdate;
    class IDraw;

    class Game
    {
    public:
        static inline Game *Instance() { return _gameInstance; }
        std::vector<IUpdate *> UpdateObjects;
        std::vector<IDraw *> DrawObjects;

        Game();
        ~Game();
        void Update();
        void Draw();

    private:
        static Game *_gameInstance;
    };
}