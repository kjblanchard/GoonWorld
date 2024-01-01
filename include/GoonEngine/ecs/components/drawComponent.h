#pragma once

typedef struct LocationComponent LocationComponent;

typedef struct DrawComponent
{
    LocationComponent* LocationComponent;
    int drawDebug;
    int sizeX;
    int sizeY;

} DrawComponent;

DrawComponent* geDrawComponentNew(LocationComponent* locationComponent);
void geDrawComponentDraw(DrawComponent* component);
void geDrawComponentSetSize(DrawComponent* component, int w, int h);