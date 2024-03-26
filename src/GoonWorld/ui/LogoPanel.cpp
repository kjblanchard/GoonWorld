#include <GoonWorld/ui/LogoPanel.hpp>
#include <GoonWorld/content/Image.hpp>
#include <GoonEngine/rectangle.h>
#include <GoonWorld/tween/Tween.hpp>
#include <GoonWorld/core/Game.hpp>
using namespace GoonWorld;
LogoPanel::LogoPanel()
    : Panel()
{

    sggImage = Image::ImageFactory("supergoongameslogo", geRectangle{50, 50, 400, 200});
    sggImage->UpdateImageAlpha(0);

    AddImage(sggImage);
    _game = Game::Instance();
    // Add tweens to handle alpha and stuff
    auto fadeInSggTween = new Tween<int>(sggImage->Alpha(), 255, 2.0, Easings::Linear);
    _game->AddTween(fadeInSggTween);
    fadeInSggTween->SetCallback(FadeInTweenCompleteFunc);
    fadeInSggTween->SetCallbackArgs(this);

    auto fadeOutSggTween = new Tween<int>(sggImage->Alpha(), 0, 2.0, Easings::Linear);

    // AddUIUpdateObject(sggImage);
    // AddUIDrawObject(sggImage);
}

void LogoPanel::FadeInTweenCompleteFunc(void *args)
{
    auto logo = static_cast<LogoPanel *>(args);
    if (!logo)
        return;
    auto fadeOutSggTween = new Tween<int>(logo->sggImage->Alpha(), 0, 2.0, Easings::Linear);
    logo->_game->AddTween(fadeOutSggTween);
}