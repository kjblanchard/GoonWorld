#include <GoonWorld/content/Image.hpp>
#include <GoonWorld/core/Content.hpp>
#include <GoonEngine/Texture2D.h>
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
    _id = Content::AddContent(this);
}

Image::~Image()
{
    Unload();
}

void Image::Load()
{
    geTexture2DGenerate(_texture, _imageName.c_str());
    _isLoaded = true;
}

void Image::Unload()
{
    geTexture2DFree(_texture);
}
