#pragma once
#include <string>
#include <GoonWorld/interfaces/ILoadContent.hpp>
typedef struct gsSfx gsSfx;
namespace GoonWorld
{
    class Sfx : public ILoadContent
    {
    public:
        static Sfx *SfxFactory(std::string sfxName);
        static inline std::string GetLoadPath(std::string &title) { return "assets/audio/" + title + ".ogg"; }
        inline bool IsLoaded() override { return _sfx != nullptr; }
        Sfx(std::string &name);
        void Play(float volume = 1.0);
        ~Sfx();
        inline std::string &GetContentName() override { return _sfxName; }
        void Load() override;
        void Unload() override;

    private:
        std::string _sfxName;
        gsSfx *_sfx = nullptr;
    };

}