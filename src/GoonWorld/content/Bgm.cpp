#include <GoonWorld/content/Bgm.hpp>
#include <GoonWorld/core/Game.hpp>
#include <GoonWorld/core/Sound.hpp>
#include <GoonWorld/core/Content.hpp>
#include <SupergoonSound/include/sound.h>
using namespace GoonWorld;

Bgm *Bgm::BgmFactory(std::string bgmName, float start, float end)
{
    auto loadPath = Bgm::GetLoadPath(bgmName);
    auto loadedText = Content::GetContent(loadPath);
    if (loadedText)
        return (Bgm *)loadedText;
    return new Bgm(loadPath, start, end);
}

Bgm::Bgm(std::string &name, float start, float end)
    : _bgmName(name), _bgmStart(start), _bgmEnd(end), _bgm(nullptr)
{
    Content::AddContent(this);
}

void Bgm::Play(int loops, float volume)
{
    gsPreLoadBgm(_bgm);
    auto config = Game::Instance()->GetSound()->GetSoundConfig();
    auto muteMultiplier = config->Muted ? 0 : 1;
    gsPlayBgm(config->MusicVolume * volume * muteMultiplier);
    if (loops == -1)
    {
        gsSetPlayerLoops(255);
    }
    else
    {
        gsSetPlayerLoops(loops);
    }
}

Bgm::~Bgm()
{
    Unload();
}

void Bgm::Load()
{
    _bgm = gsLoadBgm(_bgmName.c_str());
    _bgm->loop_begin = _bgmStart;
    _bgm->loop_end = _bgmEnd;
}

void Bgm::Unload()
{
    free(_bgm);
}