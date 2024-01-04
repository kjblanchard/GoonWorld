#include <math.h>
#include <stdio.h>
#include <GoonPhysics/aabb.h>

typedef enum {
    NoOverlap,
    OverlapTop,
    OverlapBottom,
    OverlapLeft,
    OverlapRight,
} OverlapDirection;

int gpGetOverlapDirection(gpBB* lhs, gpBB* rhs)
{
    // Check for overlap on the top side
    if (lhs->y + lhs->h > rhs->y && lhs->y < rhs->y) {
        return OverlapTop;
    }

    // Check for overlap on the bottom side
    if (lhs->y < rhs->y + rhs->h && lhs->y + lhs->h > rhs->y + rhs->h) {
        return OverlapBottom;
    }

    // Check for overlap on the left side
    if (lhs->x + lhs->w > rhs->x && lhs->x < rhs->x) {
        return OverlapLeft;
    }

    // Check for overlap on the right side
    if (lhs->x < rhs->x + rhs->w && lhs->x + lhs->w > rhs->x + rhs->w) {
        return OverlapRight;
    }

    return NoOverlap;
}


int gpIntersectBoxBox(gpBB *lhs, gpBB *rhs)
{
    if (lhs->x < rhs->x + rhs->w &&
        lhs->x + lhs->w > rhs->x &&
        lhs->y < rhs->y + rhs->h &&
        lhs->y + lhs->h > rhs->y)
    {
        return 1;
    }
    return 0;
}

void gpResolveOverlap(gpBB *lhs, gpBB *rhs)
{
    gpBB result;

    double Amin, Amax, Bmin, Bmax;

    /* Horizontal intersection */
    Amin = lhs->x;
    Amax = Amin + lhs->w;
    Bmin = rhs->x;
    Bmax = Bmin + rhs->w;
    if (Bmin > Amin)
        Amin = Bmin;
    result.x = Amin;
    if (Bmax < Amax)
        Amax = Bmax;
    result.w = Amax - Amin;

    /* Vertical intersection */
    Amin = lhs->y;
    Amax = Amin + lhs->h;
    Bmin = rhs->y;
    Bmax = Bmin + rhs->h;
    if (Bmin > Amin)
        Amin = Bmin;
    result.y = Amin;
    if (Bmax < Amax)
        Amax = Bmax;
    result.h = Amax - Amin;

    // Resolve overlap independently in x and y directions
    if (result.h > 0) {
        // Determine the direction of overlap for the y-axis
        double overlapY = (lhs->y + lhs->h / 2 < rhs->y + rhs->h / 2) ? -result.h : result.h;

        // Resolve overlap in the y-axis
        lhs->y += overlapY;
    }

    if (result.w > 0) {
        // Determine the direction of overlap for the x-axis
        double overlapX = (lhs->x + lhs->w / 2 < rhs->x + rhs->w / 2) ? -result.w : result.w;

        // Resolve overlap in the x-axis
        lhs->x += overlapX;
    }
}
void gpResolveOverlapY(gpBB *lhs, gpBB *rhs)
{
       gpBB result;

    double Amin, Amax, Bmin, Bmax;

    /* Horizontal intersection */
    Amin = lhs->x;
    Amax = Amin + lhs->w;
    Bmin = rhs->x;
    Bmax = Bmin + rhs->w;
    if (Bmin > Amin)
        Amin = Bmin;
    result.x = Amin;
    if (Bmax < Amax)
        Amax = Bmax;
    result.w = Amax - Amin;

    /* Vertical intersection */
    Amin = lhs->y;
    Amax = Amin + lhs->h;
    Bmin = rhs->y;
    Bmax = Bmin + rhs->h;
    if (Bmin > Amin)
        Amin = Bmin;
    result.y = Amin;
    if (Bmax < Amax)
        Amax = Bmax;
    result.h = Amax - Amin;

        // Resolve overlap independently in x and y directions
    // if (result.w > 0) {
    //     // Determine the direction of overlap for the x-axis
    //     double overlapX = (lhs->x + lhs->w / 2 < rhs->x + rhs->w / 2) ? -result.w : result.w;

    //     // Resolve overlap in the x-axis
    //     lhs->x += overlapX;
    // }

    if (result.h > 0) {
        // Determine the direction of overlap for the y-axis
        double overlapY = (lhs->y + lhs->h / 2 < rhs->y + rhs->h / 2) ? -result.h : result.h;

        // Resolve overlap in the y-axis
        lhs->y += overlapY;
    }


}

void gpResolveOverlapX(gpBB *lhs, gpBB *rhs)
{
       gpBB result;

    double Amin, Amax, Bmin, Bmax;

    /* Horizontal intersection */
    Amin = lhs->x;
    Amax = Amin + lhs->w;
    Bmin = rhs->x;
    Bmax = Bmin + rhs->w;
    if (Bmin > Amin)
        Amin = Bmin;
    result.x = Amin;
    if (Bmax < Amax)
        Amax = Bmax;
    result.w = Amax - Amin;

    /* Vertical intersection */
    Amin = lhs->y;
    Amax = Amin + lhs->h;
    Bmin = rhs->y;
    Bmax = Bmin + rhs->h;
    if (Bmin > Amin)
        Amin = Bmin;
    result.y = Amin;
    if (Bmax < Amax)
        Amax = Bmax;
    result.h = Amax - Amin;

        // Resolve overlap independently in x and y directions
    if (result.w > 0) {
        // Determine the direction of overlap for the x-axis
        double overlapX = (lhs->x + lhs->w / 2 < rhs->x + rhs->w / 2) ? -result.w : result.w;

        // Resolve overlap in the x-axis
        lhs->x += overlapX;
    }

    // if (result.h > 0) {
    //     // Determine the direction of overlap for the y-axis
    //     double overlapY = (lhs->y + lhs->h / 2 < rhs->y + rhs->h / 2) ? -result.h : result.h;

    //     // Resolve overlap in the y-axis
    //     lhs->y += overlapY;
    // }


}

