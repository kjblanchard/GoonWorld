#pragma once
#include <GoonEngine/ecs/system.h>

struct Entity;
struct Component;

typedef struct geContext
{
    struct Component ***ComponentArrays;
    int *ComponentCounts;    // Array to store the number of components for each type
    int *ComponentCapacity;  // Array to store the number of components for each type
    int ComponentArrayCount; // Capacity of the dynamic arrays

    struct Entity **Entities;
    int EntityCount;
    int EntityCapacity;

    System *Systems;
    int SystemCapacity;

} geContext;

geContext *geContextNew();
struct Entity *geContextEntityNew(geContext *context);
struct Entity *geGetEntityById(geContext *context, int id);
struct Component *geContextComponentNew(geContext *context, int type, void *data);
int geContextSystemNew(geContext *context, System system, int systemType);
void geContextUpdate(geContext *context, void *data);

struct Component** geContextGetComponentArrayByType(geContext* context, int type);
int geContextGetComponentArrayCountByType(geContext* context, int type);