
#pragma once

typedef struct Component Component;

typedef struct Entity
{
    int Id;
    int ComponentsBitmask;
    Component* NextComponent;
} Entity;

int geEntityAddComponent(Entity* entity, Component* component);
int geEntityHasComponent(Entity* entity, int componentBitmask);
Component* geEntityGetComponent(Entity* entity, int componentBitmask);
int geEntityRemoveComponent(Entity* entity, int componentBitmask);
