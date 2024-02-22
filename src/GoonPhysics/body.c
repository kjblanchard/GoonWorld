#include <stdio.h>
#include <GoonPhysics/body.h>
#include <GoonPhysics/overlap.h>

#define MAX_OVERLAP_BODIES 10
#define MAX_BODY_TYPES 10
// static OverlapFunc **OverlapBeginFunctions;
static OverlapFunc OverlapBeginFunctions[MAX_BODY_TYPES][MAX_BODY_TYPES];
static OverlapFunc OverlapFunctions[MAX_BODY_TYPES][MAX_BODY_TYPES];
void gpBodyAddOverlapBeginFunc(int bodyType, int overlapBodyType, OverlapFunc func)
{
    // printf("Adding func.. to pos %d, %d\n", bodyType, overlapBodyType);
    OverlapBeginFunctions[bodyType][overlapBodyType] = func;

    // OverlapBeginFunctions[bodyType][overlapBodyType](NULL,NULL);
}

void gpBodyAddOverlapFunc(int bodyType, int overlapBodyType, OverlapFunc func)
{
    OverlapFunctions[bodyType][overlapBodyType] = func;
}

gpBody *gpBodyNew(gpBB boundingBox)
{
    gpBody *body;
    body = calloc(1, sizeof(*body));
    body->overlaps = calloc(MAX_OVERLAP_BODIES, sizeof(gpOverlap *));
    body->lastFrameOverlaps = calloc(MAX_OVERLAP_BODIES, sizeof(gpOverlap *));
    body->bodyType = 1;
    body->gravityEnabled = 1;
    body->staticCollisionEnabled = 1;
    body->numOverlappingBodies = 0;
    body->velocity = gpV(0, 0);
    body->boundingBox = boundingBox;
    body->bodyNum = -1;
    return body;
}

void gpBodySetPosition(gpBody *body, gpVec pos)
{
    body->boundingBox.x = pos.x;
    body->boundingBox.y = pos.y;
}

void gpBodySetVelocity(gpBody *body, gpVec vel)
{
    body->velocity.x = vel.x;
    body->velocity.y = vel.y;
}

gpBody *gpBodyNewStatic(gpBB boundingBox)
{
    gpBody *body = gpBodyNew(boundingBox);
    body->bodyType = 0;
    return body;
}

void gpBodyAddOverlap(gpBody *body, gpBody *overlapBody, int direction)
{
    if (body->numOverlappingBodies >= MAX_OVERLAP_BODIES)
        return;
    bool newOverlap = true;
    for (size_t i = 0; i < body->lastFrameNumOverlappingBodies; i++)
    {
        if (body->lastFrameOverlaps[i].overlapBody == overlapBody)
        {
            newOverlap = false;
            break;
        }
    }
    int bodyType = body->bodyType;
    int overlapBodyType = overlapBody->bodyType;
    int currentOverlap = body->numOverlappingBodies;
    body->overlaps[body->numOverlappingBodies].overlapBody = overlapBody;
    body->overlaps[body->numOverlappingBodies].overlapDirection = direction;
    ++body->numOverlappingBodies;
    if (newOverlap)
    {
        // Fire function type
        OverlapFunc func = OverlapBeginFunctions[bodyType][overlapBodyType];
        if (func)
        {
            printf("Firing func at %d %d at loc\n", bodyType, overlapBodyType);
            func(body, overlapBody, &body->overlaps[currentOverlap]);
        }
    }
    else
    {
        OverlapFunc func = OverlapFunctions[bodyType][overlapBodyType];
        if (func)
        {
            func(body, overlapBody, &body->overlaps[currentOverlap]);
        }
    }
}

int gpBodyIsOnGround(gpBody *body)
{

    for (size_t i = 0; i < body->numOverlappingBodies; i++)
    {
        gpBody *overlap = body->overlaps[i].overlapBody;
        // If we are not a static body (type0), then continue
        if (overlap->bodyType)
        {
            continue;
        }
        if (overlap->boundingBox.y >= body->boundingBox.y + body->boundingBox.h)
        {
            return 1;
        }
    }
    return 0;
}