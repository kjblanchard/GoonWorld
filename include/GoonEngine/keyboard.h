/**
 * @file keyboard.h
 * @author Kevin Blanchard (kevin@supergoon.com)
 * @brief Handles keyboard events
 * @version 0.1
 * @date 2023-10-09
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

typedef union SDL_Event SDL_Event;

int HandleKeyboardEvent(SDL_Event *event);
void geInitializeKeyboard();
void geUpdateKeyboard();
bool geKeyJustReleased(const SDL_Scancode key);
bool geKeyJustPressed(const SDL_Scancode key);
bool geKeyHeldDown(SDL_Scancode key);