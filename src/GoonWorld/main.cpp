#include <GoonWorld/core/Game.hpp>
#include <GoonEngine/test.h>
using namespace GoonWorld;
static std::unique_ptr<Game> game;

void Update(double timeMs)
{
    game->Update(timeMs);
}

void Draw(double accumulator)
{
    game->Draw(accumulator);
}

int main()
{
    geInitializeEngine();
    geGameSetUpdateFunc(Update);
    geGameSetDrawFunc(Draw);
    game = std::make_unique<Game>();
    game->LoadLevel("level1");
    gePlayLoop();
}