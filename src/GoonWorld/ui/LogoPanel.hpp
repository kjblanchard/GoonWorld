#pragma once
#include <GoonWorld/ui/Panel.hpp>

namespace GoonWorld
{
    class Image;
    class Game;
    class LogoPanel : public Panel
    {

    public:
        LogoPanel();
        Image *sggImage = nullptr;

    private:
        static void FadeInTweenCompleteFunc(void *args);
        Game *_game;
    };
}