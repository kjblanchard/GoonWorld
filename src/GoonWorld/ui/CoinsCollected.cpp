#include <GoonWorld/ui/CoinsCollected.hpp>
#include <GoonWorld/content/Text.hpp>
#include <GoonWorld/core/Content.hpp>
#include <GoonWorld/core/Game.hpp>
#include <GoonWorld/events/Observer.hpp>
#include <GoonWorld/events/EventTypes.hpp>
using namespace GoonWorld;

CoinsCollectedUI::CoinsCollectedUI()
    : _currentCoins(0), x(150), y(10)
{
    // _coinText = new Text("Coins", Point{x, y}, geColor{255, 255, 0, 255});
    _coinText = Text::TextFactory("Coins", Point{x, y}, geColor{255, 255, 0, 255});
    Content::AddContent(_coinText);
    _coinText->Visible(true);
    for (size_t i = 0; i < 10; i++)
    {
        // auto text = new Text(std::to_string(i), gePointZero());
        auto text = Text::TextFactory(std::to_string(i), gePointZero());
        Content::AddContent(text);
        text->Visible(true);
        _loadedNumbers.push_back(text);
    }
    coinObserver = std::make_unique<Observer>((int)EventTypes::CoinCollected, [this](Event &event)
                                              { this->CoinCollectedEvent(event); });
    Game::Instance()->AddEventObserver((int)EventTypes::CoinCollected, coinObserver.get());
}

void CoinsCollectedUI::UpdateCoins(int coins)
{
    int tens = _currentCoins / 10;
    int ones = _currentCoins % 10;
    // Set old numbers to not visible
    // _loadedNumbers[tens]->Visible(false);
    // _loadedNumbers[ones]->Visible(false);
    _currentCoins = coins;
    // Set new numbers to visible
    tens = _currentCoins / 10;
    ones = _currentCoins % 10;
    // _loadedNumbers[tens]->Visible(true);
    // _loadedNumbers[ones]->Visible(true);
}

void CoinsCollectedUI::Draw()
{
    _coinText->SetX(x - _coinText->Width() - 10);
    _coinText->Draw();
    int tens = _currentCoins / 10;
    int ones = _currentCoins % 10;
    _loadedNumbers[tens]->SetLocation(Point{x, y});
    _loadedNumbers[tens]->Draw();
    _loadedNumbers[ones]->SetLocation(Point{x + _loadedNumbers[tens]->Width(), y});
    _loadedNumbers[ones]->Draw();
}

void CoinsCollectedUI::Visible(bool isVisible)
{
}

bool CoinsCollectedUI::IsVisible()
{
    return true;
}

void CoinsCollectedUI::CoinCollectedEvent(Event &event)
{
    auto coins = static_cast<int *>(event.eventArgs);
    UpdateCoins(*coins);
}