#include <GoonWorld/content/Image.hpp>
#include <GoonWorld/core/Content.hpp>
#include <GoonEngine/Texture2D.h>
// #include <GoonEngine/SdlSurface.h>
using namespace GoonWorld;

Image *Image::ImageFactory(std::string imageName)
{
    auto loadedImage = Content::GetContent(imageName);
    if (loadedImage)
        return (Image *)loadedImage;
    return new Image(imageName);
}

Image::Image(std::string &name)
    : _imageName(name), _isLoaded(false), _texture(geTexture2DNew())
{
    Content::AddContent(this);
}

Image::~Image()
{
}

void Image::Load()
{
    // surface = CreateTextureFromFile(GetLoadPath(_imageName).c_str());
    geTexture2DGenerate(_texture, _imageName.c_str());
    _isLoaded = true;
}

void Image::Unload()
{
}

void Image::Draw()
{
    if (!IsVisible())
        return;
    // geDrawTexture(surface, NULL, &_destRect, false);
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
    // geUpdateTextureAlpha(surface, alpha);
    // _alpha = alpha;
}
void Image::Update()
{
    // geUpdateTextureAlpha(surface, _alpha);
}