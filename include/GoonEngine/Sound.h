#pragma once
#include <SupergoonSound/include/sound.h>
#ifdef __cplusplus
extern "C"
{
#endif

    Bgm *BgmLoad(const char *filename, float begin, float end);
    int BgmPlay(struct Bgm *bgm, float volume);
#ifdef __cplusplus
}
#endif