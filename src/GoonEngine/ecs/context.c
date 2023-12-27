#include <GoonEngine/gnpch.h>
#include <GoonEngine/ecs/context.h>
#include <GoonEngine/ecs/entity.h>
#include <GoonEngine/ecs/component.h>
#include <GoonEngine/ecs/system.h>

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

static void CheckSystemArraySize(geContext *context)
{
    if (context->SystemCount + 1 > context->SystemCapacity)
    {
        int newSize = context->SystemCapacity * 2 + 1;
        context->Entities = realloc(context->Systems, newSize * sizeof(System *));
        if (!context->Systems)
        {
            fprintf(stderr, "Could not reallocate space for entities");
            exit(1);
        }
        context->SystemCapacity = newSize;
    }
}

static void CheckComponentArraySize(geContext *context, int type)
{
    // Resize internal arrays if this is a new type
    if (type >= context->ComponentArrayCount)
    {
        context->ComponentArrays = realloc(context->ComponentArrays, type * sizeof(Component **));
        context->ComponentCounts = realloc(context->ComponentCounts, type * sizeof(int));
        if (!context->ComponentArrays || !context->ComponentCounts)
        {
            fprintf(stderr, "Could not reallocate space for components");
            exit(1);
        }
        // change the component counts to 0 for the new types
        for (size_t i = context->ComponentArrayCount; i < type; i++)
        {
            context->ComponentCounts[i] = context->ComponentCapacity[i] = 0;
        }
    }
    // Check to see if we have enough storage in the internal array for a new one
    if (context->ComponentCounts[type] + 1 > context->ComponentCapacity[type])
    {
        int newSize = context->ComponentCapacity[type] * 2 + 1;
        context->ComponentArrays[type] = realloc(context->ComponentArrays[type], newSize * sizeof(Component *));
        if (!context->ComponentArrays[type])
        {
            fprintf(stderr, "Could not reallocate space for component array of type");
            exit(1);
        }
    }
}

geContext *geContextNew()
{
    geContext *context = calloc(1, sizeof(*context));
    return context;
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
    CheckComponentArraySize(context, type);
    context->ComponentArrays[type][context->ComponentArrayCount++] = component;
    return component;
}
int geContextSystemNew(geContext *context, System system)
{
    CheckSystemArraySize(context);
    context->Systems[context->SystemCount++] = system;
    return true;
}

void geContextUpdate(geContext* context, void* data)
{
    for (size_t i = 0; i < context->SystemCount; i++)
    {
        context->Systems[i](context, i, data);
    }

}