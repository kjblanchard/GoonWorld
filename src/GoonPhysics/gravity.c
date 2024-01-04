#include <math.h>
#include <stdio.h>
#include <GoonPhysics/gravity.h>

static void GravityConstraintX(gpBody *body, float deltaTime, gpSceneGravity *sceneGravity)
{
    if (body->velocity.x == 0)
        return;
    // Apply friction
    int onGround = gpBodyIsOnGround(body);
    float friction = onGround ? sceneGravity->sceneFriction : sceneGravity->sceneFriction * 3;
    // float friction = sceneGravity->sceneFriction;
    // body->velocity.x *= pow(sceneGravity->sceneFriction, deltaTime);
    body->velocity.x *= pow(friction, deltaTime);
    // Test if max or min is hit
    if (fabs(body->velocity.x) > sceneGravity->sceneMaxXVelocity)
    {
        body->velocity.x = copysignf(sceneGravity->sceneMaxXVelocity, body->velocity.x);
    }
    // Check if the velocity has reached a minimum threshold to stop
    if (fabs(body->velocity.x) < sceneGravity->sceneMinXVelocity)
    {
        body->velocity.x = 0;
    }
}

static void GravityConstraintY(gpBody *body, float gravityStep, gpSceneGravity *sceneGravity)
{
    double step = body->velocity.y + gravityStep;
    if (step >= 0)
    {
        if (step > sceneGravity->sceneMaxYVelocity)
            step = sceneGravity->sceneMaxYVelocity;
        if (step < sceneGravity->sceneMinYVelocity)
            step = 0;
    }
    else
    {
        if (step < -sceneGravity->sceneMaxYVelocity)
            step = -sceneGravity->sceneMaxYVelocity;
        if (step > sceneGravity->sceneMinYVelocity)
            step = 0;
    }
    body->velocity.y = (float)step;
}

void gpGravityBodyStep(gpBody *body, gpSceneGravity *sceneGravity, float deltaTime)
{
    float gravityStep = sceneGravity->sceneGravity * deltaTime;
    float frictionStep = 0.0f;
    // int friction = sceneGravity->sceneFriction;
    if (sceneGravity->sceneFriction)
    {
        frictionStep = sceneGravity->sceneFriction * deltaTime;
    }

    // Keep Y in Bounds for Max Speed
    GravityConstraintY(body, gravityStep, sceneGravity);
    // Keep X in Bounds for max Speed
    GravityConstraintX(body, deltaTime, sceneGravity);
}