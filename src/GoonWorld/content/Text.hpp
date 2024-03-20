#pragma once
#include <string>
#include <GoonWorld/interfaces/ILoadContent.hpp>
#include <GoonWorld/interfaces/IDraw.hpp>
#include <GoonEngine/color.h>
#include <GoonEngine/point.h>
#include <GoonEngine/rectangle.h>
typedef struct SDL_Texture SDL_Texture;
namespace GoonWorld
{
    class Text : public ILoadContent, public IDraw
    {
    public:
        Text(std::string stringToLoad, Point location, geColor textColor = geColor{255, 255, 255, 255});
        ~Text();
        inline std::string &GetContentName() override { return _stringToDisplay; }
        inline void SetX(int x) { _boundingBox.x = x; }
        inline void SetY(int y) { _boundingBox.x = y; }
        inline int X() { return _boundingBox.x; }
        inline void SetLocation(Point newLocation)
        {
            _boundingBox.x = newLocation.x;
            _boundingBox.y = newLocation.y;
        }
        static Text *TextFactory(std::string stringToLoad, Point location, geColor textColor = geColor{255, 255, 255, 255});
        inline int Width() { return _boundingBox.w / 2; }
        void Load() override;
        void Unload() override;
        void Draw() override;
        void Visible(bool isVisible) override;
        bool IsVisible() override;

    private:
        std::string _stringToDisplay;
        geColor _textColor;
        SDL_Texture *_loadedTexture = nullptr;
        geRectangle _boundingBox;
        bool _isVisible;
        bool _isLoaded = false;
    };

}
