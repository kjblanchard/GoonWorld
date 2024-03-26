
#pragma once
typedef struct gpOverlap gpOverlap;
#include <GoonWorld/base/GameObject.hpp>
#include <GoonWorld/interfaces/ITakeDamage.hpp>

typedef struct gpBody gpBody;

namespace GoonWorld
{
    class DebugDrawComponent;
    class RigidbodyComponent;
    class AnimationComponent;
    class Player;
    class Sfx;
    class Mushroom : public GameObject, public ITakeDamage
    {
    public:
        Mushroom(geRectangle *rect);
        void Update() override;
        void TakeDamage() override;
        void Push(bool right);

    private:
        RigidbodyComponent *_rigidbodyComponent;
        AnimationComponent *_animationComponent;
        static void StaticBodyOverlapFunc(void *instance, void *body, void *overlapBody, gpOverlap *overlap);
        static void TweenEndCallback(void *args);
        const int _moveSpeed = 25;
        bool _movingRight, _startedMoving;
        Sfx *mushroomSoundSfx;
    };

}