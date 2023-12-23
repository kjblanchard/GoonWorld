#include <GoonEngine/test.h>
#include <GoonEngine/SdlWindow.h>
#include <GoonEngine/Sound.h>

int main(int argc, char const *argv[])
{

    CreateWindowAndRenderer(300,400, "Hello");
    struct Bgm *mainBgm = BgmLoad("test.ogg", 20.397, 43.08);
    BgmPlay(mainBgm, 1.0);
    Play();
    return 0;
}
