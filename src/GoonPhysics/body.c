#include <stdio.h>
#include <GoonPhysics/body.h>
#include <GoonPhysics/overlap.h>

#define MAX_OVERLAP_BODIES 10

gpBody *gpBodyNew(gpBB boundingBox)
{
    gpBody *body;
    body = calloc(1, sizeof(*body));
    body->overlaps = calloc(MAX_OVERLAP_BODIES, sizeof(gpOverlap *));
    for (size_t i = 0; i < MAX_OVERLAP_BODIES; ++i)
    {
        body->overlaps[i] = calloc(1, sizeof(gpOverlap));
    }
    body->bodyType = 1;
    // body->bodyOnGround = 0;
    body->gravityEnabled = 1;
    body->numOverlappingBodies = 0;
    body->velocity = gpV(0, 0);
    body->boundingBox = boundingBox;
    body->bodyNum = -1;
    return body;
}

void gpBodySetPosition(gpBody* body, gpVec pos)
{
    body->boundingBox.x = pos.x;
    body->boundingBox.y = pos.y;
}

void gpBodySetVelocity(gpBody* body, gpVec vel)
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
    if (body->numOverlappingBodies < MAX_OVERLAP_BODIES)
    {
        for (size_t i = 0; i < body->numOverlappingBodies; i++)
        {
            if (body->overlaps[i]->overlapBody == overlapBody)
            {
                return;
            }
        }

        // body->overlappingBodies[body->numOverlappingBodies] = overlapBody;
        body->overlaps[body->numOverlappingBodies]->overlapBody = overlapBody;
        body->overlaps[body->numOverlappingBodies]->overlapDirection = direction;
        ++body->numOverlappingBodies;
    }
}

int gpBodyIsOnGround(gpBody *body)
{
    for (size_t i = 0; i < body->numOverlappingBodies; i++)
    {
        // gpBody *overlap = body->overlappingBodies[i];
        gpBody *overlap = body->overlaps[i]->overlapBody;
        // If we are not a static body, then continue
        if (overlap->bodyType)
            continue;
        if (overlap->boundingBox.y >= body->boundingBox.y + body->boundingBox.h)
            return 1;
            // body->bodyOnGround = 1;
    }
    // body->bodyOnGround = 0;
    // return body->bodyOnGround;
    return 0;
}