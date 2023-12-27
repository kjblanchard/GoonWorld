#pragma once
struct Entity;

typedef struct Component
{
    struct Entity* Parent;
    unsigned int Type;
    void *Data;
    struct Component* NextComponent;
} Component;

Component* geComponentNew(int type, void *data);