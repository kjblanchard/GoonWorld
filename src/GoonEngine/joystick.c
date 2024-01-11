#include <GoonEngine/gnpch.h>
#include <GoonEngine/joystick.h>
#include <GoonEngine/debug.h>

typedef struct GamePad
{
    bool lastFrameButtons[SDL_CONTROLLER_BUTTON_MAX];
    bool thisFrameButtons[SDL_CONTROLLER_BUTTON_MAX];
    int lastFrameAxis[SDL_CONTROLLER_AXIS_MAX];
    int thisFrameAxis[SDL_CONTROLLER_AXIS_MAX];
    SDL_GameController *SdlController;
} GamePad;

#define MAX_GAMEPADS 4
static int _numGamePads;
static void CountPluggedInControllers();
GamePad _connectedGamepads[MAX_GAMEPADS];

GamePad *geGamePadNew(int padNum)
{
    SDL_GameController *pad = SDL_GameControllerOpen(padNum);
    if (!pad)
    {
        LogError("Could not open gamecontroller");
        return NULL;
    }
    GamePad *gamepad = calloc(1, sizeof(*gamepad));
    gamepad->SdlController = pad;
    for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++)
    {
        gamepad->lastFrameButtons[i] = false;
        gamepad->thisFrameButtons[i] = false;
    }
}

void InitializeGoonEngineJoysticks()
{
    SDL_GameControllerEventState(SDL_ENABLE);
    CountPluggedInControllers();
}

void HandleJoystickEvent(const SDL_Event *event)
{
    switch (event->type)
    {
    case SDL_CONTROLLERDEVICEADDED:
        LogInfo("Controller added %s", event->cdevice.which);
        break;

    case SDL_CONTROLLERBUTTONDOWN:
        for (int i = 0; i < _numGamePads; i++)
        {
            if (event->cbutton.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(_connectedGamepads[i].SdlController)))
            {
                _connectedGamepads[i].thisFrameButtons[event->cbutton.button] = true;
            }
        }
        break;
    case SDL_CONTROLLERBUTTONUP:
        for (int i = 0; i < _numGamePads; i++)
        {
            if (event->cbutton.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(_connectedGamepads[i].SdlController)))
            {
                _connectedGamepads[i].thisFrameButtons[event->cbutton.button] = false;
            }
        }
        break;
    case SDL_CONTROLLERAXISMOTION:
        for (int i = 0; i < _numGamePads; i++)
        {
            if (event->cbutton.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(_connectedGamepads[i].SdlController)))
            {
                _connectedGamepads[i].thisFrameAxis[event->caxis.axis] = event->caxis.value;
            }
        }
        break;
    default:
        break;
    }
}

static void CountPluggedInControllers()
{
    int nJoysticks = SDL_NumJoysticks();
    _numGamePads = 0;
    for (int i = 0; i < nJoysticks; i++)
        if (SDL_IsGameController(i))
        {
            _numGamePads++;
            geGamePadNew(i);
        }
}

void geUpdateControllers()
{
    for (int i = 0; i < _numGamePads; i++)
    {
        memcpy(_connectedGamepads[i].lastFrameAxis, _connectedGamepads[i].thisFrameAxis, sizeof(_connectedGamepads[i].lastFrameAxis));
        memcpy(_connectedGamepads[i].lastFrameButtons, _connectedGamepads[i].thisFrameButtons, sizeof(_connectedGamepads[i].lastFrameButtons));
    }
}