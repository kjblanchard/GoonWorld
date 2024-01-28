namespace GoonEngine;
public abstract class Component
{
    public GameObject? Parent;
    public bool Enabled
    {
        get => _enabled;
        set
        {
            Debug.InfoMessage($"Enabled is {value}val");
            _enabled = value;
        }
    }
    protected bool _enabled = false;
    public virtual void OnComponentAdd(GameObject parent)
    {
        Debug.InfoMessage("On add");
        Parent = parent;
        _enabled = true;
    }
    public virtual void Update() { }
}