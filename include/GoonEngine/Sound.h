#pragma once
#include <SupergoonSound/include/sound.h>

Bgm* BgmLoad(const char* filename, float begin, float end);
int BgmPlay(struct Bgm* bgm, float volume);