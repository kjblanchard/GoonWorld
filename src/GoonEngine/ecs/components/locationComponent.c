#include <GoonEngine/gnpch.h>
#include <GoonEngine/ecs/components/locationComponent.h>

LocationComponent* gnLocationComponentNew()
{
    LocationComponent *locComponent = calloc(1, sizeof(*locComponent));
    return locComponent;
}