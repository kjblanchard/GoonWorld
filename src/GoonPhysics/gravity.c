#include <math.h>
#include <stdio.h>
#include <GoonPhysics/gravity.h>

static void GravityConstraintX(gpBody *body, double deltaTime, gpSceneGravity *sceneGravity)
{
    body->velocity.x += body->acceleration.x;
    body->acceleration.x = 0;
    if (body->velocity.x == 0)
        return;
    // Apply friction
    int onGround = gpBodyIsOnGround(body);
    double friction = onGround ? sceneGravity->sceneFriction : sceneGravity->sceneFriction * 3;
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

static void GravityConstraintY(gpBody *body, double gravityStep, gpSceneGravity *sceneGravity)
{
    body->velocity.y += body->acceleration.y;
    body->acceleration.y = 0;
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
    double gravityStep = sceneGravity->sceneGravity * deltaTime;
    double frictionStep = 0.0f;
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