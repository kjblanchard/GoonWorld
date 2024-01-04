/**
 * @file scene.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-10-30
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#include <GoonPhysics/body.h>

typedef struct gpScene
{
    float gravity;
} gpScene;

gpScene* gpInitScene(void);
void gpSceneSetGravity(gpScene *scene, float gravity);
int gpSceneAddBody(gpBody* body);
int gpSceneAddStaticBody(gpBody* body);
void gpSceneUpdate(gpScene* scene, float gameTime);
gpBody* gpSceneGetBody(int bodyRef);
