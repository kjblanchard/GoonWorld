#include <GoonWorld/platformer/Helpers.hpp>
#include <GoonWorld/ui/Panel.hpp>
#include <GoonWorld/ui/CoinsCollected.hpp>
#include <GoonWorld/ui/LevelTimer.hpp>
#include <GoonWorld/core/Level.hpp>
using namespace GoonWorld;

void Helpers::AddMarioUiToLevel(Level *level)
{
    auto panel = new Panel();
    auto coin = new CoinsCollectedUI();
    auto timer = new LevelTimer();
    panel->AddUIUpdateObject(timer);
    panel->AddUIDrawObject(timer);
    panel->AddUIDrawObject(coin);
    level->AddUiDrawObject(panel);
    level->AddUiUpdateObject(panel);
}