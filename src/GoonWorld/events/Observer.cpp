#include <GoonWorld/events/Observer.hpp>
#include <GoonWorld/core/Game.hpp>
using namespace GoonWorld;
Observer::~Observer()
{
        Game::Instance()->RemoveObserver(this);
}
// using namespace GoonWorld;
        // ~Observer();