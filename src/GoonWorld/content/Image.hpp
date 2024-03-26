#pragma once
#include <string>
#include <GoonWorld/interfaces/ILoadContent.hpp>
#include <GoonWorld/interfaces/IDraw.hpp>
#include <GoonWorld/interfaces/IUpdate.hpp>
#include <GoonEngine/rectangle.h>
typedef struct SDL_Texture SDL_Texture;
namespace GoonWorld
{
    class Image : public ILoadContent, public IDraw, public IUpdate
    {
    public:
        static Image *ImageFactory(std::string imageName, geRectangle dstRect);
        static inline std::string GetLoadPath(std::string &title) { return "assets/img/" + title + ".png"; }
        inline bool IsLoaded() override { return surface != nullptr; }
        inline int& Alpha(){return _alpha;}
        void Draw() override;
        void Visible(bool isVisible) override;
        void Update() override;
        void UpdateImageAlpha(int alpha);
        bool IsVisible() override;
        Image(std::string &name, geRectangle dstRect);
        ~Image();
        inline std::string &GetContentName() override { return _imageName; }
        void Load() override;
        void Unload() override;

    private:
        std::string _imageName;
        geRectangle _destRect;
        SDL_Texture *surface;
        bool _isVisible = true;
        int _alpha = 255;
    };

}