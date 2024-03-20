#include <GoonWorld/content/Bgm.hpp>
#include <GoonWorld/core/Game.hpp>
#include <GoonWorld/core/Sound.hpp>
#include <GoonWorld/core/Content.hpp>
using namespace GoonWorld;

Bgm *Bgm::BgmFactory(std::string bgmName)
{
    auto loadPath = Bgm::GetLoadPath(bgmName);
    auto loadedText = Content::GetContent(loadPath);
    if (loadedText)
        return (Bgm *)loadedText;
    return new Bgm(bgmName);
}

Bgm::Bgm(std::string &name)
    : _bgmName(name), _bgm(nullptr)
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
    auto config = Game::Instance()->GetSound()->GetSoundConfig();
    _bgm = gsLoadBgm(GetLoadPath(_bgmName).c_str());
    for (auto &song : config->Music)
    {
        if (song.Name.find(_bgmName) != std::string::npos)
        {
            _bgm->loop_begin = song.LoopStart;
            _bgm->loop_end = song.LoopEnd;
            return;
        }
    }
    _bgm->loop_begin = 0;
    _bgm->loop_end = 0;
}

void Bgm::Unload()
{
    free(_bgm);
}