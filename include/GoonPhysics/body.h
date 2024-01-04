/**
 * @file body.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-10-30
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#include <GoonPhysics/bb.h>
#include <GoonPhysics/vec.h>
#include <stdlib.h>


typedef struct gpBody
{
    int bodyNum;
    int bodyType;
    gpBB boundingBox;
    gpVec velocity;
    int numOverlappingBodies;
    int gravityEnabled;
    // int bodyOnGround;
    struct gpOverlap** overlaps;


} gpBody;

gpBody *gpBodyNew(gpBB boundingBox);
void gpBodySetPosition(gpBody* body, gpVec pos);
void gpBodySetVelocity(gpBody* body, gpVec vel);
void gpBodyAddOverlap(gpBody *body, gpBody *overlapBody, int direction);
int gpBodyIsOnGround(gpBody* body);
