#pragma once
int Play();
int GnInitializeEngine();
void geSetCurrentScene(void* scene);
void geGameSetUpdateFunc(void (*updateFunc)(double deltaTime));
void geGameSetDrawFunc(void (*drawFunc)());