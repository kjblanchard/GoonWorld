#include <GoonWorld/content/Image.hpp>
#include <GoonWorld/core/Content.hpp>
#include <GoonEngine/SdlSurface.h>
using namespace GoonWorld;

Image *Image::ImageFactory(std::string imageName, geRectangle dstRect)
{
    auto loadedImage = Content::GetContent(imageName);
    if (loadedImage)
        return (Image *)loadedImage;
    return new Image(imageName, dstRect);
}

Image::Image(std::string &name, geRectangle dstRect)
    : _imageName(name), _destRect(dstRect), surface(nullptr)
{
    Content::AddContent(this);
}

Image::~Image()
{
}

void Image::Load()
{
    puts("Loading image");
    surface = CreateTextureFromFile(GetLoadPath(_imageName).c_str());
}

void Image::Unload()
{
}

void Image::Draw()
{
    if (!IsVisible())
        return;
    geDrawTexture(surface, NULL, &_destRect, false);
}

void Image::Visible(bool isVisible)
{
    _isVisible = isVisible;
}

bool Image::IsVisible()
{
    return _isVisible;
}
void Image::UpdateImageAlpha(int alpha)
{
    geUpdateTextureAlpha(surface, alpha);
    _alpha = alpha;
}
void Image::Update()
{
    geUpdateTextureAlpha(surface, _alpha);
}