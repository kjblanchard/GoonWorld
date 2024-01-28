namespace GoonEngine;
public static class Mathf
{
    public static double EaseInOutQuad(float t)
    {
        return  t < 0.5f ? 2 * t * t : 1 - Math.Pow(-2 * t + 2, 2) / 2;
    }
    public static float Lerp(float a, float b, float t)
    {
        return a + (b - a) * t;
    }

}
