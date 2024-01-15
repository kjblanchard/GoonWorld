namespace GoonEngine;
public abstract class Component
{
    public GameObject? Parent;
    public virtual void OnComponentAdd(GameObject parent)
    {
        Parent = parent;
    }
}