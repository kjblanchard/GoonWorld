
#pragma once

struct Component;

typedef struct Entity
{
    int Id;
    unsigned int ComponentsBitmask;
    struct Component* NextComponent;
    struct Component* LastComponent;
} Entity;

int geEntityAddComponent(Entity* entity, struct Component* component);
int geEntityHasComponent(Entity* entity, unsigned int componentBit);
struct Component* geEntityGetComponent(Entity* entity, unsigned int componentBit);
int geEntityRemoveComponent(Entity* entity, unsigned int componentBit);
