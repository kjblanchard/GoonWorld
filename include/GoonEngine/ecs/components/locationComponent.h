#pragma once

typedef struct LocationComponent
{
    int x;
    int y;
} LocationComponent;

LocationComponent* gnLocationComponentNew();
int gnLocationComponentGetX(LocationComponent* component);
int gnLocationComponentGetY(LocationComponent* component);
void gnLocationComponentSetX(LocationComponent* component, int x);
void gnLocationComponentSetY(LocationComponent* component, int y);
void gnLocationComponentSetXY(LocationComponent* component, int x, int y);