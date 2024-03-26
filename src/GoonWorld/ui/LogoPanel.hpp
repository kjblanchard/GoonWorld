#pragma once
#include <GoonWorld/ui/Panel.hpp>

namespace GoonWorld
{
    class Image;
    class Text;
    class Game;
    class LogoPanel : public Panel
    {

    public:
        LogoPanel();
        Image *sggImage = nullptr;
        Text* textLine1 = nullptr;
        Text* textLine2 = nullptr;


    private:
        static void FadeInTweenCompleteFunc(void *args);
        static void FadeOutTweenCompleteFunc(void *args);
        static void FadeInTextTweenCompleteFunc(void *args);
        static void FadeOutTextTweenCompleteFunc(void *args);
        Game *_game;
    };
}