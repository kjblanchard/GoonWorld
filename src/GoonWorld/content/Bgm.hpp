#pragma once
#include <string>
#include <GoonWorld/interfaces/ILoadContent.hpp>
#include <SupergoonSound/include/sound.h>
// typedef struct SDL_Texture SDL_Texture;
namespace GoonWorld
{
    class Bgm : public ILoadContent
    {
    public:
        static Bgm *BgmFactory(std::string bgmName);
        static inline std::string GetLoadPath(std::string &title) { return "assets/audio/" + title + ".ogg"; }
        Bgm(std::string &name);
        void Play(int loops = -1, float volume = 1.0);
        ~Bgm();
        inline std::string &GetContentName() override { return _bgmName; }
        void Load() override;
        void Unload() override;

    private:
        std::string _bgmName;
        gsBgm *_bgm;
    };

}