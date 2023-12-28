#include <GoonEngine/gnpch.h>
#include <GoonEngine/ecs/components/locationComponent.h>

LocationComponent* gnLocationComponentNew()
{
    LocationComponent *locComponent = calloc(1, sizeof(*locComponent));
    return locComponent;
}
int gnLocationComponentGetX(LocationComponent* component)
{
    return component->x;
}
int gnLocationComponentGetY(LocationComponent* component)
{
    return component->y;
}
void gnLocationComponentSetX(LocationComponent* component, int x)
{
    component->x = x;
}
void gnLocationComponentSetY(LocationComponent* component, int y)
{
    component->y = y;
}
void gnLocationComponentSetXY(LocationComponent* component, int x, int y)
{
    component->x = x;
    component->y = y;
}