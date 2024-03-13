#include <GoonWorld/core/Sound.hpp>
#include <GoonWorld/core/Content.hpp>
#include <SupergoonSound/include/sound.h>

namespace GoonWorld
{
    Sound::Sound(AppSettings::SoundConfig &soundConfig)
        : _soundConfig(&soundConfig)
    {
    }

    bool Sound::LoadBgm(const char *title)
    {

        auto path("assets/audio/" + std::string(title) + ".ogg");
        auto bgmPtr = (gsBgm *)Content::LoadContent(ContentTypes::Bgm, path.c_str());
        if (!bgmPtr)
            return false;
        // Try and see if we have loop times for this music.
        for (auto &song : _soundConfig->Music)
        {
            if (song.Name.find(title) != std::string::npos)
            {
                bgmPtr->loop_begin = song.LoopStart;
                bgmPtr->loop_end = song.LoopEnd;
                return true;
            }
        }
        bgmPtr->loop_begin = 0;
        bgmPtr->loop_end = 0;

        return true;
    }

    void Sound::PlayBgm(const char *title, int loops)
    {
        // auto bgm = Content::GetLoadedContentOfType<gsBgm>(title);
        auto path("assets/audio/" + std::string(title) + ".ogg");
        auto bgm = Content::GetLoadedContentOfType<gsBgm>(path.c_str());
        gsPreLoadBgm(bgm);
        gsPlayBgm(_soundConfig->MusicVolume);
        if (loops == -1)
        {
            gsSetPlayerLoops(255);
        }
        else
        {
            gsSetPlayerLoops(loops);
        }
    }
}