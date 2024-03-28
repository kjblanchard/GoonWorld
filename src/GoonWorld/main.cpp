#include <GoonWorld/core/Game.hpp>
#include <GoonEngine/test.h>
using namespace GoonWorld;
static std::unique_ptr<Game> game;

static void Update(double timeMs)
{
    // game->Update(timeMs);
}

static void Draw()
{
    // game->Draw();
}

int main()
{
    geInitializeEngine();
    geGameSetUpdateFunc(Update);
    geGameSetDrawFunc(Draw);
    game = std::make_unique<Game>();
    gePlayLoop();
}