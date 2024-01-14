#pragma once

void HandleJoystickEvent(const SDL_Event *event);
void geInitializeJoysticks();
void geUpdateControllers();
bool geGamepadButtonJustReleased(const int padNum, const int button);
bool geGamepadButtonJustPressed(const int padNum, const int button);
bool geGamepadButtonHeldDown(const int padNum, const int button);