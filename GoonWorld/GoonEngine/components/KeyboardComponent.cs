namespace GoonEngine.Components;
public class KeyboardComponent
{
    public Dictionary<Models.EngineButtons, Models.SDL_Scancode> ButtonMap = new();
    public bool IsButtonDown(Models.EngineButtons button) => ButtonMap.TryGetValue(button, out var scancode) ? Api.Engine.Input.geKeyHeldDown((int)scancode) : false;
    public bool IsButtonPressed(Models.EngineButtons button) => ButtonMap.TryGetValue(button, out var scancode) ? Api.Engine.Input.geKeyJustPressed((int)scancode) : false;
    public bool IsButtonReleased(Models.EngineButtons button) => ButtonMap.TryGetValue(button, out var scancode) ? Api.Engine.Input.geKeyJustReleased((int)scancode) : false;
}