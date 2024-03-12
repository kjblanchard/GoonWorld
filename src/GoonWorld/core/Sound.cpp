#include <GoonWorld/core/Sound.hpp>

namespace GoonWorld
{
    Sound::Sound(AppSettings::SoundConfig &soundConfig)
        : _soundConfig(&soundConfig)
    {
    }

    bool Sound::LoadBgm(const char *title)
    {
        if (LoadedMusic.find(title) != LoadedMusic.end())
            return true;

        for (auto &song : _soundConfig->Music)
        {
            if (song.Title.find(title) != std::string::npos)
            {
                auto bgmPtr = BgmLoad(song.Name.c_str(), song.LoopStart, song.LoopEnd);
                LoadedMusic[title] = bgmPtr;
                return true;
            }
        }
        return false;
    }

    void Sound::PlayBgm(const char *title, int loops)
    {
        auto bgm_it = LoadedMusic.find(title);
        if (bgm_it == LoadedMusic.end())
            return;
        BgmPlay(bgm_it->second, _soundConfig->MusicVolume);
        if (loops == -1)
        {
            geSetPlayerLoops(255);
        }
        else
        {
            geSetPlayerLoops(loops);
        }
    }
}