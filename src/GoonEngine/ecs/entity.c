#include <GoonEngine/gnpch.h>
#include <GoonEngine/ecs/entity.h>
#include <GoonEngine/ecs/component.h>

int geEntityAddComponent(Entity *entity, Component *component)
{
    if ((entity->ComponentsBitmask & (1 << component->Type)) != 0)
    {
        // Component already added
        return false;
    }
    entity->ComponentsBitmask |= (1 << component->Type);
    component->Parent = entity;
    if (entity->LastComponent)
    {
        entity->LastComponent->NextComponent = component;
        entity->LastComponent = component;
        return true;
    }
    else
    {
        entity->NextComponent = entity->LastComponent = component;
    }
    return true;
}

int geEntityHasComponent(Entity *entity, unsigned int componentBit)
{
    printf("Searching for component %d\n", componentBit);
    if ((entity->ComponentsBitmask & (1 << componentBit)) == 0)
    {
        printf("Does not have it? %d\n", componentBit);
        return false;
    }
    printf("Has it %d\n", componentBit);
    return true;
}

Component *geEntityGetComponent(Entity *entity, unsigned int componentBit)
{
    if (!geEntityHasComponent(entity, componentBit))
        return NULL;
    Component *nextComponent = entity->NextComponent;
    while (nextComponent)
    {
        if (nextComponent->Type == componentBit)
            return nextComponent;
        nextComponent = nextComponent->NextComponent;
    }
    return NULL;
}

int geEntityRemoveComponent(Entity *entity, unsigned int componentBit)
{
    return false;
}