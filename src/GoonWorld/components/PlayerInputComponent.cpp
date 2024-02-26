#include <GoonWorld/models/AppSettings.hpp>
#include <GoonWorld/components/PlayerInputComponent.hpp>
#include <GoonWorld/core/Game.hpp>
#include <GoonEngine/keyboard.h>
#include <GoonEngine/joystick.h>
using namespace GoonWorld;

PlayerInputComponent::PlayerInputComponent(int playerNum)
    : Component((int)ComponentTypes::Input), _playerNum(playerNum)
{
    LoadInputFromConfig();
}
void PlayerInputComponent::LoadInputFromConfig()
{
    auto &settings = *Game::Instance()->GameSettings;
    auto &keyboardSettings = settings.KeyboardConfig.PlayerButtonKeyVector[_playerNum];
    auto &controllerSettings = settings.ControllerConfig.PlayerButtonVector[_playerNum];
    for (auto i = 0; i < keyboardSettings.size(); ++i)
    {
        for (int j = 0; j < keyboardSettings[i].size(); ++j)
        {
            KeyboardButtonMap[(GameControllerButton)i] = keyboardSettings[i][j];
        }
        JoystickButtons.push_back(controllerSettings[i]);
    }
}

bool PlayerInputComponent::IsButtonDownOrHeld(GameControllerButton button)
{
    return IsButtonDown(button) || IsButtonPressed(button);
}
bool PlayerInputComponent::IsButtonDown(GameControllerButton button)
{
    auto iter = KeyboardButtonMap.find(button);
    if (iter == KeyboardButtonMap.end())
        return false;
    auto thing = *iter;
    return geKeyHeldDown(thing.second) || geGamepadButtonHeldDown(_playerNum, (int)JoystickButtons[(int)button]);
}
bool PlayerInputComponent::IsButtonPressed(GameControllerButton button)
{
    auto iter = KeyboardButtonMap.find(button);
    return (iter != KeyboardButtonMap.end()) && (geKeyJustPressed(iter->second) || geGamepadButtonJustPressed(_playerNum, (int)JoystickButtons[(int)button]));
}
bool PlayerInputComponent::IsButtonReleased(GameControllerButton button)
{
    auto iter = KeyboardButtonMap.find(button);
    return (iter != KeyboardButtonMap.end()) && (geKeyJustReleased(iter->second) || geGamepadButtonJustReleased(_playerNum, (int)JoystickButtons[(int)button]));
}