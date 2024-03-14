#include <GoonWorld/core/Sound.hpp>
#include <GoonWorld/core/Content.hpp>
#include <SupergoonSound/include/sound.h>

namespace GoonWorld
{
    Sound::Sound(AppSettings::SoundConfig &soundConfig)
        : _soundConfig(&soundConfig)
    {
        gsInitializeSound();
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

    void Sound::PlayBgm(const char *title, int loops, float volume)
    {
        // auto bgm = Content::GetLoadedContentOfType<gsBgm>(title);
        auto path("assets/audio/" + std::string(title) + ".ogg");
        auto bgm = Content::GetLoadedContentOfType<gsBgm>(path.c_str());
        gsPreLoadBgm(bgm);
        auto muteMultiplier = _soundConfig->Muted ? 0 : 1;
        gsPlayBgm(_soundConfig->MusicVolume * volume * muteMultiplier);
        if (loops == -1)
        {
            gsSetPlayerLoops(255);
        }
        else
        {
            gsSetPlayerLoops(loops);
        }
    }
    bool Sound::LoadSfx(const char *title)
    {
        auto path("assets/audio/" + std::string(title) + ".ogg");
        auto sfx = (gsSfx *)Content::LoadContent(ContentTypes::Sfx, path.c_str());
        return sfx ? true : false;
    }

    void Sound::LoadSfx(std::initializer_list<const char *> sfxList)
    {
        for (auto sfx : sfxList)
        {
            LoadSfx(sfx);
        }
    }
    void Sound::PlaySfx(const char *title, float volume)
    {
        auto path("assets/audio/" + std::string(title) + ".ogg");
        auto sfx = Content::GetLoadedContentOfType<gsSfx>(path.c_str());
        auto muteMultiplier = _soundConfig->Muted ? 0 : 1;
        if (!sfx)
            return;
        gsPlaySfxOneShot(sfx, volume * _soundConfig->SfxVolume * muteMultiplier);
    }
}