namespace GoonEngine;
public class TagComponent : Component
{
    // public TagComponent(IntPtr context, params string[] tags)
    public TagComponent(params string[] tags)
    {
        _componentDataPointer = Api.Components.TagComponent.gnTagComponentNew();
        _ecsComponentDataPtr = ECS.NewComponent(ComponentTypes.TAG_COMPONENT, ComponentPtr);
        foreach (var tag in tags)
        {
            AddTag(tag);
        }
    }
    public bool HasTag(string tag) => Api.Components.TagComponent.gnTagComponentHasTag(ComponentPtr, tag);
    public bool AddTag(string tag) => Api.Components.TagComponent.gnTagComponentAddTag(ComponentPtr, tag);
}