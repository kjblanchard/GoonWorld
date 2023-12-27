#pragma once
typedef struct Entity Entity;

typedef struct Component
{
    Entity* Parent;
    int Type;
    void *Data;
    struct Component* NextComponent;
} Component;

Component* geComponentNew(int type, void *data);