#pragma once
#include <GoonWorld/base/Component.hpp>

namespace GoonWorld
{
    /**
     * @brief These should match SDL gamecontroller buttons for now, as we just translate to that
     */
    enum class GameControllerButton
    {
        A,
        B,
        X,
        Y,
        BACK,
        GUIDE,
        START,
        LEFTSTICK,
        RIGHTSTICK,
        LEFTSHOULDER,
        RIGHTSHOULDER,
        DPAD_UP,
        DPAD_DOWN,
        DPAD_LEFT,
        DPAD_RIGHT,
        MISC1,    /* Xbox Series X share button, PS5 microphone button, Nintendo Switch Pro capture button, Amazon Luna microphone button */
        PADDLE1,  /* Xbox Elite paddle P1 (upper left, facing the back) */
        PADDLE2,  /* Xbox Elite paddle P3 (upper right, facing the back) */
        PADDLE3,  /* Xbox Elite paddle P2 (lower left, facing the back) */
        PADDLE4,  /* Xbox Elite paddle P4 (lower right, facing the back) */
        TOUCHPAD, /* PS4/PS5 touchpad button */
        MAX
    };
    class PlayerInputComponent : public Component
    {

    public:
        PlayerInputComponent(int playerNum);
        bool IsButtonDownOrHeld(GameControllerButton button);
        bool IsButtonDown(GameControllerButton button);
        bool IsButtonPressed(GameControllerButton button);
        bool IsButtonReleased(GameControllerButton button);

    private:
        void LoadInputFromConfig();
        int _playerNum;
        std::map<GameControllerButton, int> KeyboardButtonMap;
        std::vector<int> JoystickButtons;
    // public Dictionary<SdlGameControllerButton, SdlScancodes> ButtonMap = new();
    };
}