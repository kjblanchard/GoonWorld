#pragma once
#include <vector>
#include <memory>
#include <GoonWorld/interfaces/IDraw.hpp>

namespace GoonWorld
{
    class Text;
    class Event;
    class Observer;
    class CoinsCollectedUI : public IDraw
    {
    public:
        CoinsCollectedUI();
        void UpdateCoins(int coins);
        int _currentCoins;
        int x;
        int y;
        Text *_coinText;
        int numberOffset;
        std::vector<Text *> _loadedNumbers;
        void Draw(double accum) override;
        void Visible(bool isVisible) override;
        bool IsVisible() override;
        void CoinCollectedEvent(Event &event);
        std::unique_ptr<Observer> coinObserver;
    };

}