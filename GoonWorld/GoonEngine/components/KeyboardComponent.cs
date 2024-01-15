namespace GoonEngine.Components;
public class KeyboardComponent : Component
{
    public Dictionary<SdlGameControllerButton, SdlScancodes> ButtonMap = new();
    public int[] joystickMap;
    public int playerNum = 0;
    public bool IsButtonDown(SdlGameControllerButton button) =>
        (ButtonMap.TryGetValue(button, out var scancode) ? Api.Engine.Input.geKeyHeldDown((int)scancode) : false) ||
        Api.Engine.Input.geGamepadButtonHeldDown(playerNum, joystickMap[(int)button]);
    public bool IsButtonPressed(SdlGameControllerButton button) =>
        (ButtonMap.TryGetValue(button, out var scancode) ? Api.Engine.Input.geKeyJustPressed((int)scancode) : false) ||
        Api.Engine.Input.geGamepadButtonJustPressed(playerNum, joystickMap[(int)button]);
    public bool IsButtonReleased(SdlGameControllerButton button) =>
        (ButtonMap.TryGetValue(button, out var scancode) ? Api.Engine.Input.geKeyJustReleased((int)scancode) : false) ||
        Api.Engine.Input.geGamepadButtonJustReleased(playerNum, joystickMap[(int)button]);

    public void LoadControllerSettingsFromConfig(int playerNum)
    {
        var playerKeyboard = Game.Global.Config.Config.keyboardConfig[playerNum];
        var playerGamepad = Game.Global.Config.Config.controllerConfig[playerNum];
        joystickMap = new int[playerGamepad.Count];
        // For each controller button
        for (int i = 0; i < playerKeyboard.Count; i++)
        {
            // For each mapped button (once implemented, for now break after first)
            for (int j = 0; j < playerKeyboard[i].Count; j++)
            {
                ButtonMap[(SdlGameControllerButton)i] = (SdlScancodes)playerKeyboard[i][j];
                break;
            }
            joystickMap[i] = playerGamepad[i];
        }
    }
}
public enum SdlGameControllerButton
{
    A,
    B,
    X,
    Y,
    Back,
    Guide,
    Start,
    LeftStick,
    RightStick,
    LeftShoulder,
    RightShoulder,
    DPadUp,
    DPadDown,
    DPadLeft,
    DPadRight,
    LeftTrigger,
    RightTrigger,
    Max
}

public enum SdlScancodes
{
    SDL_SCANCODE_UNKNOWN = 0,

    SDL_SCANCODE_A = 4,
    SDL_SCANCODE_B = 5,
    SDL_SCANCODE_C = 6,
    SDL_SCANCODE_D = 7,
    SDL_SCANCODE_E = 8,
    SDL_SCANCODE_F = 9,
    SDL_SCANCODE_G = 10,
    SDL_SCANCODE_H = 11,
    SDL_SCANCODE_I = 12,
    SDL_SCANCODE_J = 13,
    SDL_SCANCODE_K = 14,
    SDL_SCANCODE_L = 15,
    SDL_SCANCODE_M = 16,
    SDL_SCANCODE_N = 17,
    SDL_SCANCODE_O = 18,
    SDL_SCANCODE_P = 19,
    SDL_SCANCODE_Q = 20,
    SDL_SCANCODE_R = 21,
    SDL_SCANCODE_S = 22,
    SDL_SCANCODE_T = 23,
    SDL_SCANCODE_U = 24,
    SDL_SCANCODE_V = 25,
    SDL_SCANCODE_W = 26,
    SDL_SCANCODE_X = 27,
    SDL_SCANCODE_Y = 28,
    SDL_SCANCODE_Z = 29,

    SDL_SCANCODE_1 = 30,
    SDL_SCANCODE_2 = 31,
    SDL_SCANCODE_3 = 32,
    SDL_SCANCODE_4 = 33,
    SDL_SCANCODE_5 = 34,
    SDL_SCANCODE_6 = 35,
    SDL_SCANCODE_7 = 36,
    SDL_SCANCODE_8 = 37,
    SDL_SCANCODE_9 = 38,
    SDL_SCANCODE_0 = 39,

    // ... (Add other SDL_SCANCODES as needed)

    SDL_SCANCODE_RETURN = 40,
    SDL_SCANCODE_ESCAPE = 41,
    SDL_SCANCODE_BACKSPACE = 42,
    SDL_SCANCODE_TAB = 43,
    SDL_SCANCODE_SPACE = 44,

    // ... (Add more SDL_SCANCODES)

    SDL_SCANCODE_UP = 82,
    SDL_SCANCODE_DOWN = 81,
    SDL_SCANCODE_RIGHT = 79,
    SDL_SCANCODE_LEFT = 80,

    // ... (Add more SDL_SCANCODES)
}
