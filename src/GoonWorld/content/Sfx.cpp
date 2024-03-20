#include <GoonWorld/content/Sfx.hpp>
#include <GoonWorld/core/Content.hpp>
#include <GoonWorld/core/Game.hpp>
#include <GoonWorld/core/Sound.hpp>
#include <SupergoonSound/include/sound.h>

using namespace GoonWorld;
Sfx *Sfx::SfxFactory(std::string sfxName)
{
    auto loadPath = Sfx::GetLoadPath(sfxName);
    auto loadedText = Content::GetContent(loadPath);
    if (loadedText)
        return (Sfx *)loadedText;
    // return new Sfx(sfxName);
    return new Sfx(loadPath);
}

Sfx::Sfx(std::string &name)
    : _sfxName(name), _sfx(nullptr)
{
    Content::AddContent(this);
}
void Sfx::Play(float volume)
{
    auto config = Game::Instance()->GetSound()->GetSoundConfig();
    auto muteMultiplier = config->Muted ? 0 : 1;
    gsPlaySfxOneShot(_sfx, volume * config->SfxVolume * muteMultiplier);
}
Sfx::~Sfx()
{
}
void Sfx::Load()
{
    // auto name = GetLoadPath(_sfxName);
    // _sfx = gsLoadSfxHelper(name.c_str());
    _sfx = gsLoadSfxHelper(_sfxName.c_str());
}
void Sfx::Unload()
{
    gsUnloadSfx(_sfx);
}