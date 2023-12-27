#pragma once
#include <GoonEngine/ecs/system.h>

typedef struct Entity Entity;
typedef struct Component Component;

typedef struct geContext
{
    Component ***ComponentArrays;
    int *ComponentCounts;    // Array to store the number of components for each type
    int *ComponentCapacity;    // Array to store the number of components for each type
    int ComponentArrayCount; // Capacity of the dynamic arrays

    Entity** Entities;
    int EntityCount;
    int EntityCapacity;

    System *Systems;
    int SystemCount;
    int SystemCapacity;

} geContext;

geContext* geContextNew();
Entity* geContextEntityNew(geContext* context);
Entity* geGetEntityById(geContext* context, int id);
Component* geContextComponentNew(geContext* context, int type, void* data);
int geContextSystemNew(geContext* context, System system);
void geContextUpdate(geContext* context, void* data);