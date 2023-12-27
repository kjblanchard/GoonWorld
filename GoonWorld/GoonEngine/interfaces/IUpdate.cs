namespace GoonEngine;
/// <summary>
/// Used to implement the Update function
/// </summary>
interface IUpdate
{
    public static double DeltaTime;
    public virtual void Update(){}
}