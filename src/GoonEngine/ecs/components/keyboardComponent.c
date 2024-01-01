#include <GoonEngine/gnpch.h>
#include <GoonEngine/keyboard.h>
#include <GoonEngine/ecs/components/keyboardComponent.h>

KeyboardComponent *KeyboardComponentNew()
{
    KeyboardComponent *kbc = calloc(1, sizeof(*kbc));
    return kbc;
}
void SetControllerMapButton(KeyboardComponent *keyboardComponent, int buttonToSet, int sdlScancode)
{
    if (buttonToSet < 0 || buttonToSet > ebMax)
        return;
    keyboardComponent->ControllerMap.Buttons[buttonToSet] = sdlScancode;
}
int CheckIfButtonReleased(KeyboardComponent *keyboardComponent, int buttonToCheck)
{
    // int result = geKeyJustReleased(keyboardComponent->ControllerMap.Buttons[buttonToCheck]);
    // printf("Result %d", result);
    return geKeyJustReleased(keyboardComponent->ControllerMap.Buttons[buttonToCheck]);
}
int CheckIfButtonPressed(KeyboardComponent *keyboardComponent, int buttonToCheck)
{
    // int result = geKeyJustPressed(keyboardComponent->ControllerMap.Buttons[buttonToCheck]);
    // printf("Result %d", result);
    return geKeyJustPressed(keyboardComponent->ControllerMap.Buttons[buttonToCheck]);
}
int CheckIfButtonHeld(KeyboardComponent *keyboardComponent, int buttonToCheck)
{
    // int result = geKeyHeldDown(keyboardComponent->ControllerMap.Buttons[buttonToCheck]);
    // printf("Result %d", result);
    return geKeyHeldDown(keyboardComponent->ControllerMap.Buttons[buttonToCheck]);
}
int CheckIfButtonDown(KeyboardComponent *keyboardComponent, int buttonToCheck)
{
    // int result = geKeyHeldDown(keyboardComponent->ControllerMap.Buttons[buttonToCheck]) || geKeyHeldDown(keyboardComponent->ControllerMap.Buttons[buttonToCheck]);
    // printf("Result %d", result);
    return geKeyHeldDown(keyboardComponent->ControllerMap.Buttons[buttonToCheck]) || geKeyHeldDown(keyboardComponent->ControllerMap.Buttons[buttonToCheck]);
}