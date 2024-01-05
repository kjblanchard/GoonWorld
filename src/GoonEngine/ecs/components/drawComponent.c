#include <GoonEngine/gnpch.h>
#include <GoonEngine/ecs/components/drawComponent.h>
#include <GoonEngine/ecs/components/locationComponent.h>

extern SDL_Renderer *g_pRenderer;

DrawComponent* geDrawComponentNew(LocationComponent* locationComponent)
{
    DrawComponent* drawComp = calloc(1, sizeof(*drawComp));
    drawComp->LocationComponent = locationComponent;
    drawComp->drawDebug = true;
    return drawComp;
}

void geDrawComponentSetSize(DrawComponent* component, int w, int h)
{
    component->sizeX = w;
    component->sizeY = h;
}
void geDrawComponentSetWidth(DrawComponent* component, int w)
{
    component->sizeX = w;
}

void geDrawComponentSetHeight(DrawComponent* component, int h)
{
    component->sizeY = h;
}

void geDrawComponentDraw(DrawComponent* component)
{
    SDL_Rect drawRect;
    drawRect.x = component->LocationComponent->x;
    drawRect.y = component->LocationComponent->y;
    drawRect.w = component->sizeX;
    drawRect.h = component->sizeY;
    SDL_SetRenderDrawColor(g_pRenderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(g_pRenderer, &drawRect);
    SDL_SetRenderDrawColor(g_pRenderer, 100, 100, 100, 255);
}