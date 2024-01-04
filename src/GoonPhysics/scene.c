#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GoonPhysics/scene.h>
#include <GoonPhysics/body.h>
#include <GoonPhysics/gravity.h>
#include <GoonPhysics/aabb.h>
#include <GoonPhysics/overlap.h>

// Rigidbodies
static int _currentNumBodies = 0;
static int _currentCapacityBodies = 4;
static gpBody **_currentBodies;
// Static Bodies
static int _currentNumStaticBodies = 0;
static int _currentCapacityStaticBodies = 4;
static gpBody **_currentStaticBodies;

static void ApplyYVelocity(gpBody *body, float gameTime);
static void ApplyXVelocity(gpBody *body, float gameTime);
static void CheckForNonStaticOverlaps(gpBody *body, int direction);

void gpSceneUpdate(gpScene *scene, float gameTime)
{
    gpSceneGravity sceneGravity;
    sceneGravity.sceneGravity = scene->gravity;
    // sceneGravity.sceneFriction = 2.25f;
    sceneGravity.sceneFriction = 0.25;
    sceneGravity.sceneMaxXVelocity = 100000;
    sceneGravity.sceneMaxYVelocity = 100000;
    sceneGravity.sceneMinYVelocity = 0.001;
    sceneGravity.sceneMinXVelocity = 10;

    for (size_t i = 0; i < _currentNumBodies; i++)
    {
        gpBody *body = _currentBodies[i];
        if (!body->gravityEnabled)
            continue;
        gpGravityBodyStep(body, &sceneGravity, gameTime);
        // Remove overlap bodies after gravity is updated.
        body->numOverlappingBodies = 0;
        ApplyYVelocity(body, gameTime);
        ApplyXVelocity(body, gameTime);
        // CheckForNonStaticOverlaps(body, gameTime);
    }
}

static void CheckForNonStaticOverlaps(gpBody *body, int direction)
{
    // Check for non static bodies
    for (size_t i = 0; i < _currentNumBodies; i++)
    {
        gpBody *overlapBody = _currentBodies[i];
        if (overlapBody == body)
            continue;
        int intersect = gpIntersectBoxBox(&body->boundingBox, &overlapBody->boundingBox);
        if (intersect)
        {
            gpBodyAddOverlap(body, overlapBody, direction);
        }
    }
}

static void ApplyYVelocity(gpBody *body, float gameTime)
{
    float initialYStep = body->velocity.y * gameTime;
    float iterYStep = initialYStep;
    float stepSize = (int)initialYStep != 0 ? initialYStep > 0 ? 1 : -1 : initialYStep;
    int shouldStep = stepSize != 0 ? 1 : 0;
    while (shouldStep)
    {
        float bodyInitialY = body->boundingBox.y;
        body->boundingBox.y += stepSize;
        // Check for collisions for each static body
        // If it is a blocking body, then we should set shouldStep to False
        int direction = stepSize > 0 ? gpOverlapDown : gpOverlapUp;
        for (size_t i = 0; i < _currentNumStaticBodies; i++)
        {
            gpBody *staticBody = _currentStaticBodies[i];
            int intersect = gpIntersectBoxBox(&body->boundingBox, &staticBody->boundingBox);
            if (intersect)
            {
                gpResolveOverlapY(&body->boundingBox, &staticBody->boundingBox);
                gpBodyAddOverlap(body, staticBody, direction);
                shouldStep = 0;
            }
        }
        CheckForNonStaticOverlaps(body, direction);

        // For body in bodies, if collides,
        // then send out notify for subscribers with info of collision bounding box and body num
        if (!shouldStep)
        {
            // If we are set to be blocked by the other body,
            // then set should step to 0, and revert body back to initial
            // body->boundingBox.y -= stepSize;
            body->velocity.y = 0.0;
            continue;
        }
        iterYStep -= stepSize;
        if (iterYStep && fabs(iterYStep) < 1)
        {
            // We have a partial step remaining, so add that in a final round.
            stepSize = iterYStep;
        }
        else if (!iterYStep)
        {
            shouldStep = 0;
        }
    }
}

static void ApplyXVelocity(gpBody *body, float gameTime)
{
    float initialXStep = body->velocity.x * gameTime;
    float iterXStep = initialXStep;
    float stepSize = (int)initialXStep != 0 ? initialXStep > 0 ? 1 : -1 : initialXStep;
    int shouldStep = stepSize != 0 ? 1 : 0;
    int direction = stepSize > 0 ? gpOverlapRight : gpOverlapLeft;
    while (shouldStep)
    {
        float bodyInitialX = body->boundingBox.x;
        body->boundingBox.x += stepSize;
        // Check for collisions for each body
        // For body in bodies, if collides,
        // then send out notify for subscribers with info of collision bounding box and body num
        // If it is a blocking body, then we should set shouldStep to False
        for (size_t i = 0; i < _currentNumStaticBodies; i++)
        {
            gpBody *staticBody = _currentStaticBodies[i];
            int intersect = gpIntersectBoxBox(&body->boundingBox, &staticBody->boundingBox);
            if (intersect)
            {

                gpResolveOverlapX(&body->boundingBox, &staticBody->boundingBox);
                gpBodyAddOverlap(body, staticBody, direction);
                shouldStep = 0;
            }
        }

        if (!shouldStep)
        {
            // If we are set to be blocked by the other body,
            // then set should step to 0, and revert body back to initial
            // body->boundingBox.x -= stepSize;
            body->velocity.x = 0.0;
            continue;
        }
        iterXStep -= stepSize;
        if (iterXStep && fabs(iterXStep) < 1)
        {
            // We have a partial step remaining, so add that in a final round.
            stepSize = iterXStep;
        }
        else if (!iterXStep)
        {
            shouldStep = 0;
        }
    }
    CheckForNonStaticOverlaps(body, direction);
}

gpScene *gpInitScene(void)
{
    gpScene *scene = malloc(sizeof(*scene));
    _currentBodies = calloc(_currentCapacityBodies, _currentCapacityBodies * sizeof(gpBody *));
    _currentStaticBodies = calloc(_currentCapacityStaticBodies, _currentCapacityBodies * sizeof(gpBody *));
    return scene;
}

void gpSceneSetGravity(gpScene *scene, float gravity)
{
    scene->gravity = gravity;
}

int gpSceneAddBody(gpBody *body)
{
    if (_currentNumBodies > _currentCapacityBodies / 2)
    {
        _currentBodies = realloc(_currentBodies, _currentCapacityBodies * 2 * sizeof(gpBody *));
        if (_currentBodies == NULL)
        {
            fprintf(stderr, "Couldn't reallocate to increase body size, what the");
        }
        _currentCapacityBodies *= 2;
    }
    _currentBodies[_currentNumBodies] = body;
    body->bodyNum = _currentNumBodies;
    ++_currentNumBodies;
    return body->bodyNum;
}
int gpSceneAddStaticBody(gpBody *body)
{
    if (_currentNumStaticBodies > _currentCapacityStaticBodies / 2)
    {
        _currentStaticBodies = realloc(_currentStaticBodies, _currentCapacityStaticBodies * 2 * sizeof(gpBody *));
        if (_currentStaticBodies == NULL)
        {
            fprintf(stderr, "Couldn't reallocate to increase static body size, what the");
        }
        _currentCapacityStaticBodies *= 2;
    }
    // _currentBodies[_currentNumBodies] = body;
    _currentStaticBodies[_currentNumStaticBodies] = body;
    ++_currentNumStaticBodies;
    return _currentNumStaticBodies - 1;
}

gpBody *gpSceneGetBody(int bodyRef)
{
    if (bodyRef < _currentNumBodies && _currentBodies[bodyRef])
    {
        return _currentBodies[bodyRef];
    }
}
