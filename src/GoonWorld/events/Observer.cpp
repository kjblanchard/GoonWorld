#include <GoonWorld/events/Observer.hpp>
#include <GoonWorld/core/Game.hpp>
using namespace GoonWorld;
Observer::~Observer()
{
        if (!Game::Instance())
                return;
        Game::Instance()->RemoveObserver(this);
}