#include <GoonWorld/core/Game.hpp>
#include <GoonWorld/platformer/Helpers.hpp>
#include <GoonWorld/ui/Panel.hpp>
#include <GoonWorld/ui/CoinsCollected.hpp>
#include <GoonWorld/ui/LevelTimer.hpp>
#include <GoonWorld/ui/BoxUi.hpp>
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
Observer *Helpers::_levelEndObserver = new Observer((int)EventTypes::LevelEnd, [](Event &event)
                                                    { Helpers::HandleEvent(event); });
Observer *Helpers::_playerPowerupCompleteObserver = new Observer((int)EventTypes::PlayerPowerupComplete, [](Event &event)
                                                                 { Helpers::HandleEvent(event); });

void Helpers::AddMarioEventObserverFunctions()
{
    Game::Instance()->AddEventObserver((int)EventTypes::PlayerWin, _playerWinObserver);
    Game::Instance()->AddEventObserver((int)EventTypes::PlayerDie, _playerDieObserver);
    Game::Instance()->AddEventObserver((int)EventTypes::PlayerPowerup, _playerBigObserver);
    Game::Instance()->AddEventObserver((int)EventTypes::LevelStart, _loadLevelObserver);
    Game::Instance()->AddEventObserver((int)EventTypes::LevelEnd, _loadLevelObserver);
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

    case (int)EventTypes::LevelEnd:
        // TODO this should be diff
        Game::Instance()->SetNextTiledLevelIfLevel();
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
    // auto box = new BoxUi(geRectangle{0, 0, 64, 64});
    panel->AddUIDrawObject(timer);
    panel->AddUIDrawObject(coin);
    // panel->AddUIDrawObject(box);
    level->AddUiPanel(panel);
}