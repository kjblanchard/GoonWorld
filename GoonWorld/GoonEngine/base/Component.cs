namespace GoonEngine;
public abstract class Component
{
    public GameObject? Parent;
    public bool Enabled = false;
    public virtual void OnComponentAdd(GameObject parent)
    {
        Parent = parent;
        Enabled = true;
    }
    public virtual void Update(){}
}