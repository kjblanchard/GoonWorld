#pragma once
#include <SDL2/SDL_keyboard.h>

typedef enum EngineButtons
{
    ebA = 0,
    ebB,
    ebX,
    ebY,
    ebStart,
    ebSelect,
    ebUp,
    ebRight,
    ebDown,
    ebLeft,
    ebMax,

} EngineButtons;

typedef struct KeyboardMap
{
    int Buttons[ebMax];
} KeyboardMap;

typedef struct KeyboardComponent
{
    KeyboardMap ControllerMap;

} KeyboardComponent;

KeyboardComponent *KeyboardComponentNew();
void SetControllerMapButton(KeyboardComponent *keyboardComponent, int buttonToSet, int sdlScancode);
int CheckIfButtonReleased(KeyboardComponent *keyboardComponent, int buttonToCheck);
int CheckIfButtonPressed(KeyboardComponent *keyboardComponent, int buttonToCheck);
int CheckIfButtonHeld(KeyboardComponent *keyboardComponent, int buttonToCheck);
int CheckIfButtonDown(KeyboardComponent *keyboardComponent, int buttonToCheck);