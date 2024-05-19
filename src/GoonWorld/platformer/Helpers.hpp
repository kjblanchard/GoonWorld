#pragma once
#include <GoonWorld/events/Event.hpp>
#include <GoonWorld/events/Observer.hpp>
namespace GoonWorld
{
    class Level;

    class Helpers
    {
    public:
        static void AddMarioUiToLevel(Level *level);
        static void AddMarioEventObserverFunctions();
        inline static bool ShouldEnemyUpdate() { return _shouldEnemyUpdate; }

    private:
        static void HandleEvent(Event &event);
        static bool _shouldEnemyUpdate;
        static Observer *_playerWinObserver, *_playerDieObserver, *_playerBigObserver, *_loadLevelObserver, *_levelEndObserver, *_playerPowerupCompleteObserver;
    };

}