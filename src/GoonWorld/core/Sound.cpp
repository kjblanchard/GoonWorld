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

}