#pragma once
#include <map>
#include <GoonWorld/models/AppSettings.hpp>
namespace GoonWorld
{
    class Sound
    {
    public:
        Sound(AppSettings::SoundConfig &soundConfig);
        bool LoadBgm(const char *title);
        void PlayBgm(const char *title, int loops = -1);
        void UnloadBgms();

    private:
        AppSettings::SoundConfig *_soundConfig;
    };
}