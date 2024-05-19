#include <GoonWorld/ui/LevelTimer.hpp>
#include <GoonWorld/content/Text.hpp>
#include <GoonWorld/core/Content.hpp>
#include <GoonWorld/core/Game.hpp>
#include <GoonWorld/events/Observer.hpp>
#include <GoonWorld/events/EventTypes.hpp>
#include <GoonWorld/platformer/Helpers.hpp>
using namespace GoonWorld;

LevelTimer::LevelTimer()
    : currentTimeSeconds(0), x(300), y(10)
{
    _timeText = Text::TextFactory("Time", Point{x, y}, geColor{255, 255, 0, 255});
    Content::AddContent(_timeText);
    _timeText->Visible(true);
    for (size_t i = 0; i < 10; i++)
    {
        auto text = Text::TextFactory(std::to_string(i), gePointZero());
        Content::AddContent(text);
        _loadedNumbers.push_back(text);
    }
}

void LevelTimer::UpdateTime(int coins)
{
    currentTimeTotalSeconds = coins;
    currentTimeSeconds = coins;
}

void LevelTimer::Draw()
{
    _timeText->SetX(x - _timeText->Width() - 10);
    _timeText->Draw();
    int minutes = currentTimeSeconds / 60;
    int totalSeconds = currentTimeSeconds % 60;
    int secondsTens = totalSeconds / 10;
    int secondsOnes = currentTimeSeconds % 10;
    _loadedNumbers[minutes]->SetLocation(Point{x, y});
    _loadedNumbers[minutes]->Draw();
    _loadedNumbers[secondsTens]->SetLocation(Point{x + _loadedNumbers[minutes]->Width(), y});
    _loadedNumbers[secondsTens]->Draw();
    _loadedNumbers[secondsOnes]->SetLocation(Point{_loadedNumbers[secondsTens]->X() + _loadedNumbers[secondsTens]->Width(), y});
    _loadedNumbers[secondsOnes]->Draw();
}

void LevelTimer::Update()
{
    if (!Helpers::ShouldEnemyUpdate())
        return;
    currentTimeTotalSeconds += Game::Instance()->DeltaTime().GetTotalSeconds();
    auto time = (int)currentTimeTotalSeconds;
    if (time != currentTimeSeconds)
        UpdateTime(time);
}

void LevelTimer::Visible(bool isVisible)
{
}

bool LevelTimer::IsVisible()
{
    return true;
}
