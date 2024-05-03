#include <GoonWorld/ui/LogoPanel.hpp>
#include <GoonWorld/content/Image.hpp>
#include <GoonEngine/rectangle.h>
#include <GoonWorld/tween/Tween.hpp>
#include <GoonWorld/core/Game.hpp>
#include <GoonWorld/content/Text.hpp>
using namespace GoonWorld;
LogoPanel::LogoPanel()
    : Panel()
{

    sggImage = Image::ImageFactory("supergoongameslogo", geRectangle{50, 50, 400, 200});
    sggImage->UpdateImageAlpha(0);

    textLine1 = Text::TextFactory("Made with SDL2 and..", Point{50, 50});
    textLine2 = Text::TextFactory("OpenAL Freetype Libpng C++", Point{50, textLine1->Height() + 50});
    // make text fully transparent for now.
    textLine1->UpdateTextAlpha(0);
    textLine2->UpdateTextAlpha(0);

    AddText(textLine1);
    AddText(textLine2);
    AddImage(sggImage);
    _game = Game::Instance();
    // Add tweens to handle alpha and stuff
    auto fadeInSggTween = new Tween<int>(sggImage->Alpha(), 255, 2.0, Easings::Linear);
    _game->AddTween(fadeInSggTween);
    fadeInSggTween->SetCallback(FadeInTweenCompleteFunc);
    fadeInSggTween->SetCallbackArgs(this);
}

LogoPanel::~LogoPanel()
{
}

void LogoPanel::FadeInTweenCompleteFunc(void *args)
{
    auto logo = static_cast<LogoPanel *>(args);
    if (!logo)
        return;
    auto fadeOutSggTween = new Tween<int>(logo->sggImage->Alpha(), 0, 2.0, Easings::Linear);
    logo->_game->AddTween(fadeOutSggTween);
    fadeOutSggTween->SetCallback(FadeOutTweenCompleteFunc);
    fadeOutSggTween->SetCallbackArgs(logo);
}

void LogoPanel::FadeOutTweenCompleteFunc(void *args)
{
    auto logo = static_cast<LogoPanel *>(args);
    if (!logo)
        return;
    auto fadeInTextTween = new Tween<int>(logo->textLine1->Alpha(), 255, 2.0, Easings::Linear);
    auto fadeInText2Tween = new Tween<int>(logo->textLine2->Alpha(), 255, 2.0, Easings::Linear);
    // auto newY = logo->textLine1->Height();
    logo->textLine2->SetY(logo->textLine2->Y() + logo->textLine1->Height() + 10);
    logo->_game->AddTween(fadeInTextTween);
    logo->_game->AddTween(fadeInText2Tween);

    fadeInTextTween->SetCallback(FadeInTextTweenCompleteFunc);
    fadeInTextTween->SetCallbackArgs(logo);
}
void LogoPanel::FadeInTextTweenCompleteFunc(void *args)
{
    auto logo = static_cast<LogoPanel *>(args);
    if (!logo)
        return;
    auto fadeInTextTween = new Tween<int>(logo->textLine1->Alpha(), 0, 2.0, Easings::Linear);
    auto fadeInText2Tween = new Tween<int>(logo->textLine2->Alpha(), 0, 2.0, Easings::Linear);

    logo->_game->AddTween(fadeInTextTween);
    logo->_game->AddTween(fadeInText2Tween);
    fadeInTextTween->SetCallback(FadeOutTextTweenCompleteFunc);
    fadeInTextTween->SetCallbackArgs(logo);
}

void LogoPanel::FadeOutTextTweenCompleteFunc(void *args)
{
    auto logo = static_cast<LogoPanel *>(args);
    if (!logo)
        return;
    std::string levelName = "level1";
    // logo->_game->StartGameLevel(levelName);
    logo->_game->ChangeToTiledLevel(levelName);
}