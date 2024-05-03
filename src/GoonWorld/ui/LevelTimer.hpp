#pragma once
#include <vector>
// #include <GoonWorld/interfaces/IDraw.hpp>
// #include <GoonWorld/interfaces/IUpdate.hpp>
#include <GoonWorld/base/UiObject.hpp>

namespace GoonWorld
{
    class Text;
    class LevelTimer : public UiObject
    {
    public:
        LevelTimer();
        void UpdateTime(int timeSeconds);
        float currentTimeTotalSeconds = 0;
        int currentTimeSeconds = 0;
        int x;
        int y;
        Text *_timeText;
        int numberOffset;
        std::vector<Text *> _loadedNumbers;
        void Update() override;
        void Draw() override;
        void Visible(bool isVisible) override;
        bool IsVisible() override;
    };

}