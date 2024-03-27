#pragma once
#include <string>
#include <GoonWorld/interfaces/ILoadContent.hpp>
#include <GoonWorld/interfaces/IDraw.hpp>
#include <GoonWorld/interfaces/IUpdate.hpp>
#include <GoonEngine/color.h>
#include <GoonEngine/point.h>
#include <GoonEngine/rectangle.h>
typedef struct SDL_Texture SDL_Texture;
namespace GoonWorld
{
    class Text : public ILoadContent, public IDraw, public IUpdate
    {
    public:
        Text(std::string stringToLoad, Point location, geColor textColor = geColor{255, 255, 255, 255});
        ~Text();
        inline std::string &GetContentName() override { return _stringToDisplay; }
        inline void SetX(int x) { _boundingBox.x = x; }
        inline void SetY(int y) { _boundingBox.y = y; }
        inline int &Alpha() { return _alpha; }
        inline int X() { return _boundingBox.x; }
        inline int Y() { return _boundingBox.y; }
        void Update() override;
        inline bool IsLoaded() override { return _isLoaded; }
        inline void SetLocation(Point newLocation)
        {
            _boundingBox.x = newLocation.x;
            _boundingBox.y = newLocation.y;
        }
        static Text *TextFactory(std::string stringToLoad, Point location, geColor textColor = geColor{255, 255, 255, 255});
        inline int Width() { return _boundingBox.w / 2; }
        inline int Height() { return _boundingBox.h / 2; }
        void UpdateTextAlpha(int alpha);
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
        int _alpha = 255;
        bool _isVisible = true;
        bool _isLoaded = false;
    };

}
