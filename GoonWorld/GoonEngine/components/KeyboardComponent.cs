using GoonEngine.Models;

namespace GoonEngine;
public class KeyboardComponent : Component
{
    public KeyboardComponent()
    {
        _componentDataPointer = Api.Components.KeyboardComponent.KeyboardComponentNew();
        _ecsComponentDataPtr = ECS.NewComponent(ComponentTypes.KEYBOARD_COMPONENT, ComponentPtr);
    }
    public bool GetButtonDown(EngineButtons button) => Api.Components.KeyboardComponent.CheckIfButtonDown(_componentDataPointer, (int)button);
    public bool GetButtonHeld(EngineButtons button) => Api.Components.KeyboardComponent.CheckIfButtonHeld(_componentDataPointer, (int)button);
    public bool GetButtonPressed(EngineButtons button) => Api.Components.KeyboardComponent.CheckIfButtonPressed(_componentDataPointer, (int)button);
    public bool GetButtonReleased(EngineButtons button) => Api.Components.KeyboardComponent.CheckIfButtonReleased(_componentDataPointer, (int)button);
    public void SetControllerButton(EngineButtons buttonToBeMapped, SDL_Scancode button) => Api.Components.KeyboardComponent.SetControllerMapButton(_componentDataPointer, (int)buttonToBeMapped, (int)button);
}