namespace GoonEngine;
public interface IUpdate
{
    public bool Enabled {get;}
    public virtual void Update() { }
}