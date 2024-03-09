#pragma once
#include <SupergoonSound/include/sound.h>
#ifdef __cplusplus
extern "C"
{
#endif

    Bgm *BgmLoad(const char *filename, float begin, float end);
    int BgmPlay(Bgm *bgm, float volume);
    Sfx *geSfxLoad(const char *filename);
int geSfxPlay(Sfx *sfx, float volume);
int geSetPlayerLoops(int loops);
#ifdef __cplusplus
}
#endif