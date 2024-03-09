#include <GoonEngine/gnpch.h>
#include <GoonEngine/Sound.h>
#include <SupergoonSound/include/sound.h>

static SDL_Event event;

Bgm *BgmLoad(const char *filename, float begin, float end)
{
    // TODO this shouldn't be pumping out events here, remove this.  This is here to remove the lag from loading initially, but we could be losing events here.
    while (SDL_PollEvent(&event))
    {
    }
    Bgm *bgm = gsLoadBgm(filename, begin, end);
    if (!bgm)
    {
        fprintf(stderr, "Could not load BGM %s", filename);
        return NULL;
    }
    int result = gsPreLoadBgm(bgm);
    if (!result)
    {
        fprintf(stderr, "Could not preload BGM %s", filename);
        return NULL;
    }
    // Returns BGM pointer, or nil, which should be free'd afterwards.
    return bgm;
}

Sfx *geSfxLoad(const char *filename)
{
    Sfx *sfx = gsLoadSfxHelper(filename);
    if (!sfx)
    {
        // LogError("Could not load Sfx %s", filename);
        return NULL;
    }
    int result = gsLoadSfx(sfx);
    if (!result)
    {
        fprintf(stderr, "Could not load sfx %s", filename);
    }
    return sfx;
}
int geSfxPlay(Sfx *sfx, float volume)
{
    gsPlaySfxOneShot(sfx, volume);
    return 0;
}

int BgmPlay(Bgm *bgm, float volume)
{
    int result = gsPreLoadBgm(bgm);
    if (!result)
    {
        fprintf(stderr, "Could not play BGM %s", bgm->bgm_name);
    }
    gsPlayBgm(volume);
    return 0;
}

// static int DestroyBgm(lua_State *L)
// {
//     // Arg1: Bgm*
//     if (!lua_islightuserdata(L, 1))
//     {
//         LogError("Bad argument passed into PlayBGm, expected a userdata ptr");
//         return 0;
//     }
//     Bgm *bgm = (Bgm *)lua_touserdata(L, 1);
//     if (!bgm)
//     {
//         LogError("Pointer passed to playbgm is not able to be casted to a bgm");
//         return 0;
//     }
//     free(bgm);
//     return 0;
// }