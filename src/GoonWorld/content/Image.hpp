#pragma once
#include <string>
#include <GoonWorld/interfaces/ILoadContent.hpp>
#include <GoonEngine/rectangle.h>
typedef struct geTexture2D geTexture2D;
namespace GoonWorld
{
    class Image : public ILoadContent
    {
    public:
        static Image *ImageFactory(std::string imageName);
        static inline std::string GetLoadPath(std::string &title) { return "assets/img/" + title + ".png"; }
        inline geTexture2D *GetTexture() { return _texture; }
        inline bool IsLoaded() override { return _isLoaded; }
        inline unsigned int GetAssetID() const override { return _id; };
        inline void SetAssetID(unsigned int ID) override { _id = ID; };
        Image(std::string &name);
        ~Image();
        inline std::string &GetContentName() override { return _imageName; }
        void Load() override;
        void Unload() override;

    private:
        std::string _imageName;
        bool _isLoaded;
        geTexture2D *_texture;
        unsigned int _id;
    };

}