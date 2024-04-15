#include <GoonWorld/core/Game.hpp>
#include <GoonEngine/game.h>
using namespace GoonWorld;
static std::unique_ptr<Game> game;

void Update(double timeMs)
{
    game->Update(timeMs);
}

void Draw()
{
    game->Draw();
}

int main()
{
    geInitializeEngine();
    geGameSetUpdateFunc(Update);
    geGameSetDrawFunc(Draw);
    game = std::make_unique<Game>();
    gePlayLoop();
}