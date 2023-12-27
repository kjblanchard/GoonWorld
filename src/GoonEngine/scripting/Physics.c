#include <GoonEngine/gnpch.h>
#include <GoonPhysics/GoonPhysics.h>
#include <GoonPhysics/aabb.h>
#include <GoonPhysics/overlap.h>

int AddBodyToScene(SDL_Rect* bodyRect, int bodyType)
{
    gpBB bb = gpBBNew(bodyRect->x, bodyRect->y, bodyRect->w, bodyRect->h);
    gpBody *body = gpBodyNew(bb);
    int bodyNum = gpSceneAddBody(body);
    body->bodyType = bodyType;
    return bodyNum;
}

int AddStaticBodyToScene(SDL_Rect* bodySdlRect)
{
    gpBB bb = gpBBNew(bodySdlRect->x, bodySdlRect->y, bodySdlRect->w, bodySdlRect->h);
    gpBody *body = gpBodyNew(bb);
    body->bodyType = 0;
    int bodyNum = gpSceneAddStaticBody(body);
    return bodyNum;
}

gpBody *GetBodyFromScene(int bodyNum)
{
    gpBody *body = gpSceneGetBody(bodyNum);
    return body;
}

struct overlap
{
    gpBody* body;
    int direction;
};
// static int GetOverlappingBodies(int bodyNum, int bodyType)
// {
//     gpBody *body = GetBodyFromScene(L, bodyNum);
//     if (!body)
//         return 1;
//     lua_newtable(L);
//     int tableListLoc = lua_gettop(L);
//     int bodiesAdded = 0;
//     for (size_t i = 0; i < body->numOverlappingBodies; i++)
//     {
//         // gpBody *overlapBody = body->overlappingBodies[i];
//         gpBody *overlapBody = body->overlaps[i]->overlapBody;
//         int direction = body->overlaps[i]->overlapDirection;
//         if (overlapBody->bodyType != bodyType)
//             continue;
//         // Create a new table
//         lua_newtable(L);
//         lua_pushnumber(L, overlapBody->bodyNum);
//         lua_setfield(L, -2, "body");
//         lua_pushnumber(L, direction);
//         lua_setfield(L, -2, "direction");
//         lua_rawseti(L, tableListLoc, ++bodiesAdded);
//     }
//     return 1;
// }

int AddForceToBody(int bodyNum, float forceX, float forceY)
{
    gpBody *body = gpSceneGetBody(bodyNum);
    if (!body)
    {
        return 0;
    }
    body->velocity.x += forceX;
    body->velocity.y += forceY;
    return 0;
}

int ToggleBodyGravity(int bodyNum, int gravity)
{
    gpBody *body = GetBodyFromScene(bodyNum);
    if (!body)
        return 1;
    body->gravityEnabled = gravity;
}

int SetBodyType(int bodyNum, int bodyType)
{
    gpBody *body = GetBodyFromScene(bodyNum);
    if (!body)
        return 1;
    body->bodyType = bodyType;
}

int IsBodyOnGround(int bodyNum)
{
    gpBody *body = gpSceneGetBody(bodyNum);
    if (!body)
    {
        return 1;
    }
     return gpBodyIsOnGround(body);
}

int GetOverlapDirection(int bodyNum, int overlapBodyNum)
{
    gpBody *body = gpSceneGetBody(bodyNum);
    gpBody *overlapBody = gpSceneGetBody(overlapBodyNum);
    if (!body || !overlapBody)
    {
        return 1;
    }
    return gpGetOverlapDirection(&body->boundingBox, &overlapBody->boundingBox);
}

gpVec GetBodyCoordinates(int bodyRef)
{
    gpBody *body = gpSceneGetBody(bodyRef);
    if (!body)
    {
        // LogWarn("Could not get body num %d from the physics scene", bodyRef);
        return gpV(0,0);
    }
    return gpV(body->boundingBox.x, body->boundingBox.y);
}

int SetBodyPos(int bodyRef, gpVec pos)
{
    gpBody *body = gpSceneGetBody(bodyRef);
    if (!body)
    {
        return 1;
    }
    body->boundingBox.x = pos.x;
    body->boundingBox.y = pos.y;
    return 0;
}

gpVec GetBodyVelocity(int bodyRef)
{
    gpBody *body = gpSceneGetBody(bodyRef);
    // if (!body)
    // {
    //     return 1;
    return gpV(body->velocity.x, body->velocity.y);
}

int SetBodyVelocity(int bodyRef, gpVec velocity)
{
    gpBody *body = gpSceneGetBody(bodyRef);
    if (!body)
    {
        return 1;
    }
    body->velocity.x = velocity.x;
    body->velocity.y = velocity.y;
}