#pragma once
#include <map>
#include <GoonWorld/models/AppSettings.hpp>
#include <GoonEngine/Sound.h>
namespace GoonWorld
{
    class Sound
    {
    public:
        Sound(AppSettings::SoundConfig &soundConfig);
        bool LoadBgm(const char *title);
        void PlayBgm(const char *title, int loops = -1);
        std::unordered_map<std::string, Bgm *> LoadedMusic;

    private:
        AppSettings::SoundConfig *_soundConfig;
    };
}