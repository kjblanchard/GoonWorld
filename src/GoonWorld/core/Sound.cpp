#include <GoonWorld/core/Sound.hpp>
#include <GoonWorld/content/Bgm.hpp>
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
        Bgm::BgmFactory(title);
    }

    void Sound::PlayBgm(std::string title, int loops, float volume)
    {
        auto bgmPath = Bgm::GetLoadPath(title);
        auto thing = dynamic_cast<Bgm *>(Content::GetContent(bgmPath));
        if (thing)
            thing->Play(loops, volume);
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