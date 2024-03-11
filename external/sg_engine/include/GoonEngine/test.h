#pragma once
struct InputEvent;
#include <GoonEngine/InputEvent.h>
#ifdef __cplusplus
extern "C"
{
#endif
    int Play();
    int ExitEngine();
    int GnInitializeEngine();
    void geSetCurrentScene(void *scene);
    void geGameSetUpdateFunc(void (*updateFunc)(double deltaTime));
    void geGameSetDrawFunc(void (*drawFunc)());
#ifdef __cplusplus
}
#endif