#pragma once
#include <GoonWorld/base/Component.hpp>
#include <GoonEngine/point.h>
#include <GoonPhysics/body.h>
#include <GoonWorld/BodyTypes.hpp>

struct SDL_Rect;
struct gpBody;
namespace GoonWorld
{
    class RigidbodyComponent : public Component
    {
    public:
        RigidbodyComponent(SDL_Rect *rect);
        ~RigidbodyComponent();
        static void PhysicsUpdate();
        // bool IsOnGround();
        inline void GravityEnabled(bool isEnabled) { _body->gravityEnabled = isEnabled; }
        inline bool IsOnGround() { return gpBodyIsOnGround(_body); }
        inline void SizeChange(Point newSize)
        {
            _body->boundingBox.w = newSize.x;
            _body->boundingBox.h = newSize.y;
        }
        inline gpVec &Velocity() { return _body->velocity; }
        inline gpVec &Acceleration() { return _body->acceleration; }
        inline gpVec &MaxVelocity() { return _body->maxVelocity; }
        inline gpVec &Friction() { return _body->friction; }
        inline void SetStaticBody(bool isStatic) { _static = isStatic; }
        inline void SetCollidesWithStaticBody(bool doesCollide) { _body->staticCollisionEnabled = doesCollide; }
        inline void SetBodyType(BodyTypes bodyType) { _body->bodyType = (int)bodyType; }
        inline void SetBodyType(int bodyType) { _body->bodyType = bodyType; }
        // TODO move this
        gpBody *_body;

    private:
        void OnComponentAdd(GameObject &parent) override;
        static std::vector<RigidbodyComponent *> _currentRigidbodies;
        int _bodyNum;
        long long _isOnGroundCached;
        bool _isOnGround, _static;
    };
}