
#pragma once
typedef struct gpOverlap gpOverlap;
#include <GoonWorld/base/GameObject.hpp>
#include <GoonWorld/interfaces/ITakeDamage.hpp>

typedef struct SDL_Rect SDL_Rect;

namespace GoonWorld
{
    class DebugDrawComponent;
    class RigidbodyComponent;
    class AnimationComponent;
    class Player;
    class Mushroom : public GameObject, public ITakeDamage
    {
    public:
        Mushroom(SDL_Rect *rect);
        void Update() override;
        void TakeDamage() override;
        void Push(bool right);

    private:
        DebugDrawComponent *_debugDrawComponent;
        RigidbodyComponent *_rigidbodyComponent;
        AnimationComponent *_animationComponent;
        void MushroomStaticBodyOverlap(gpOverlap *overlap);
        const int _moveSpeed = 25;
        bool _movingRight, _startedMoving;

        int _contents;
    };

}