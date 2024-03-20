#pragma once
#include <map>
#include <GoonWorld/models/AppSettings.hpp>
namespace GoonWorld
{
    class Sound
    {
    public:
        Sound(AppSettings::SoundConfig &soundConfig);
        inline const AppSettings::SoundConfig *GetSoundConfig() { return _soundConfig; }
        // bool LoadBgm(const char *title);
        // void PlayBgm(std::string title, int loops = 255, float volume = 1.0);
        // void PlayBgm(const char *title, int loops = -1, float volume = 1.0f);
        // bool LoadSfx(const char *title);
        // void LoadSfx(std::initializer_list<const char *> sfxList);
        // void PlaySfx(const char *title, float volume = 1.0f);
        void UnloadBgms();
        void UnloadSfx();

    private:
        AppSettings::SoundConfig *_soundConfig;
    };
}