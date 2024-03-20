#include <GoonWorld/content/Text.hpp>
#include <GoonWorld/core/Content.hpp>
#include <GoonEngine/text.h>
#include <GoonEngine/SdlSurface.h>
using namespace std;
using namespace GoonWorld;

Text::Text(string stringToLoad, Point location, geColor textColor)
    : _stringToDisplay(stringToLoad), _textColor(textColor), _boundingBox(geRectangleZero()), _isVisible(true)
{
    _boundingBox.x = location.x;
    _boundingBox.y = location.y;
}

Text * Text::TextFactory(std::string stringToLoad, Point location, geColor textColor)
{
    auto loadedText = Content::GetContent(stringToLoad);
    if (loadedText)
        return (Text *)loadedText;
    return new Text(stringToLoad, location, textColor);
}

Text::~Text()
{
    printf("Text unloading\n");
    Unload();
}

void Text::Draw()
{
    if (!IsVisible())
        return;
    geRectangle testBox = _boundingBox;
    testBox.w /= 2;
    testBox.h /= 2;
    geDrawTexture(_loadedTexture, NULL, &_boundingBox, false);
}

void Text::Visible(bool isVisible)
{
    _isVisible = isVisible;
}

bool Text::IsVisible()
{
    return _isVisible;
}

void Text::Load()
{
    if (_isLoaded)
        return;
    Point textureDimensions = gePointZero();
    _loadedTexture = geCreateTextureForString(_stringToDisplay.c_str(), &textureDimensions, _textColor);
    _boundingBox.w = textureDimensions.x;
    _boundingBox.h = textureDimensions.y;
    _isLoaded = true;
}

void Text::Unload()
{
    DestroyTexture(_loadedTexture);
}