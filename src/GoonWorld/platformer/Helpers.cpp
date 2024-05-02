#include <GoonWorld/core/Game.hpp>
#include <GoonWorld/platformer/Helpers.hpp>
#include <GoonWorld/ui/Panel.hpp>
#include <GoonWorld/ui/CoinsCollected.hpp>
#include <GoonWorld/ui/LevelTimer.hpp>
#include <GoonWorld/core/Level.hpp>
#include <GoonPhysics/scene.h>
#include <GoonWorld/events/EventTypes.hpp>
#include <memory>
using namespace GoonWorld;

bool Helpers::_shouldEnemyUpdate = false;

Observer *Helpers::_playerWinObserver = new Observer((int)EventTypes::PlayerWin, [](Event &event)
                                                     { Helpers::HandleEvent(event); });
Observer *Helpers::_playerDieObserver = new Observer((int)EventTypes::PlayerDie, [](Event &event)
                                                     { Helpers::HandleEvent(event); });
Observer *Helpers::_playerBigObserver = new Observer((int)EventTypes::PlayerPowerup, [](Event &event)
                                                     { Helpers::HandleEvent(event); });
Observer *Helpers::_loadLevelObserver = new Observer((int)EventTypes::LevelStart, [](Event &event)
                                                     { Helpers::HandleEvent(event); });
Observer *Helpers::_playerPowerupCompleteObserver = new Observer((int)EventTypes::PlayerPowerupComplete, [](Event &event)
                                                     { Helpers::HandleEvent(event); });

void Helpers::AddMarioEventObserverFunctions()
{
    Game::Instance()->AddEventObserver((int)EventTypes::PlayerWin, _playerWinObserver);
    Game::Instance()->AddEventObserver((int)EventTypes::PlayerDie, _playerDieObserver);
    Game::Instance()->AddEventObserver((int)EventTypes::PlayerPowerup, _playerBigObserver);
    Game::Instance()->AddEventObserver((int)EventTypes::LevelStart, _loadLevelObserver);
    Game::Instance()->AddEventObserver((int)EventTypes::PlayerPowerupComplete, _playerPowerupCompleteObserver);
}
void Helpers::HandleEvent(Event &event)
{
    switch (event.eventType)
    {
    case (int)EventTypes::PlayerPowerup:
    case (int)EventTypes::PlayerDie:
    case (int)EventTypes::PlayerWin:
        _shouldEnemyUpdate = false;
        break;

    default:
    case (int)EventTypes::PlayerPowerupComplete:
    case (int)EventTypes::LevelStart:
        _shouldEnemyUpdate = true;
        break;
    }
}
void Helpers::AddMarioUiToLevel(Level *level)
{
    auto panel = new Panel();
    auto coin = new CoinsCollectedUI();
    auto timer = new LevelTimer();
    panel->AddUIUpdateObject(timer);
    panel->AddUIDrawObject(timer);
    panel->AddUIDrawObject(coin);
    level->AddUiPanel(panel);
}