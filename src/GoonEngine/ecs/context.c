#include <GoonEngine/gnpch.h>
#include <GoonEngine/debug.h>
#include <GoonEngine/ecs/context.h>
#include <GoonEngine/ecs/entity.h>
#include <GoonEngine/ecs/component.h>
#include <GoonEngine/ecs/system.h>

static int maxTypeEncountered = -1;
geContext* g_Context;
extern void (*DrawUpdateFunc)();

static void CheckEntityArraySize(geContext *context)
{
    if (context->EntityCount + 1 > context->EntityCapacity)
    {
        int newSize = context->EntityCapacity * 2 + 1;
        context->Entities = realloc(context->Entities, newSize * sizeof(Entity *));
        if (!context->Entities)
        {
            fprintf(stderr, "Could not reallocate space for entities");
            exit(1);
        }
        context->EntityCapacity = newSize;
    }
}

static void CheckSystemArraySize(geContext *context, int systemNum)
{
    int newSize = systemNum + 1;
    if (newSize > context->SystemCapacity)
    {
        context->Systems = realloc(context->Systems, newSize * sizeof(System *));
        if (!context->Systems)
        {
            fprintf(stderr, "Could not reallocate space for entities");
            exit(1);
        }
        context->SystemCapacity = newSize;
    }
}

static void AllocateSpaceForNewComponentType(geContext *context, int type)
{
    context->ComponentArrays = realloc(context->ComponentArrays, (type + 1) * sizeof(Component **));
    context->ComponentCounts = realloc(context->ComponentCounts, (type + 1) * sizeof(int));
    context->ComponentCapacity = realloc(context->ComponentCapacity, (type + 1) * sizeof(int));
    if (!context->ComponentArrays || !context->ComponentCounts)
    {
        fprintf(stderr, "Could not reallocate space for components");
        exit(1);
    }
    // change the component counts to 0 for the new types
    for (size_t i = context->ComponentArrayCount; i <= type; i++)
    {
        context->ComponentCounts[i] = context->ComponentCapacity[i] = 0;
        context->ComponentArrays[type] = calloc(1, sizeof(Component *));
    }
    context->ComponentArrayCount = type + 1;
}

static void CheckComponentArraySize(geContext *context, int type)
{
    if (context->ComponentCounts[type] + 1 > context->ComponentCapacity[type])
    {
        int newSize = context->ComponentCapacity[type] * 2 + 1;
        printf("Going to make new size of %d for type %d\n", newSize, type);
        LogError("Going to make new size of %d for type %d\n", newSize, type);
        context->ComponentArrays[type] = realloc(context->ComponentArrays[type], newSize * sizeof(Component *));
        if (!context->ComponentArrays[type])
        {
            fprintf(stderr, "Could not reallocate space for component array of type");
            exit(1);
        }
        context->ComponentCapacity[type] = newSize;
    }
}

geContext *geContextNew()
{
    geContext *context = calloc(1, sizeof(*context));
    context->ComponentArrays = calloc(1, sizeof(Component **));
    context->ComponentCounts = calloc(1, sizeof(int));
    context->ComponentCapacity = calloc(1, sizeof(int));
    return g_Context = context;
}

void geContextSetDrawFunc(void(*drawFunc)())
{
    DrawUpdateFunc = drawFunc;
}

Entity *geContextEntityNew(geContext *context)
{
    Entity *entity = calloc(1, sizeof(*entity));
    entity->Id = context->EntityCount;
    CheckEntityArraySize(context);
    context->Entities[context->EntityCount++] = entity;
    return entity;
}
Entity *geGetEntityById(geContext *context, int id)
{
    if (id < 0 || id > context->EntityCount)
        return NULL;
    return context->Entities[id];
}

Component *geContextComponentNew(geContext *context, int type, void *data)
{
    Component *component = calloc(1, sizeof(*component));
    component->Type = type;
    component->Data = data;
    printf("Creating for context %x and type %d\n", context, type);
    if (type > maxTypeEncountered)
    {
        AllocateSpaceForNewComponentType(context, type);
        maxTypeEncountered = type;
    }
    CheckComponentArraySize(context, type);
    // context->ComponentArrays[type][context->ComponentArrayCount++] = component;
    context->ComponentArrays[type][context->ComponentCounts[type]++] = component;
    return component;
}
int geContextSystemNew(geContext *context, System system, int systemType)
{
    CheckSystemArraySize(context, systemType);
    context->Systems[systemType] = system;
    return true;
}

void geContextUpdate(geContext *context, void *data)
{
    for (size_t i = 0; i < context->SystemCapacity; i++)
    {
        if (!context->Systems[i])
            continue;
        context->Systems[i](context, i, data);
    }
}

struct Component **geContextGetComponentArrayByType(geContext *context, int type)
{
    return context->ComponentArrays[type];
}
int geContextGetComponentArrayCountByType(geContext *context, int type)
{
    return context->ComponentCounts[type];
}