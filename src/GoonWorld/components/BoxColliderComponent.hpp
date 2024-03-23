#pragma once
#include <GoonWorld/base/Component.hpp>
#include <GoonEngine/point.h>
// #include <GoonPhysics/body.h>
#include <GoonPhysics/boxCollider.h>
#include <GoonWorld/BodyTypes.hpp>
#include <GoonEngine/rectangle.h>
#include <GoonPhysics/body.h>
#include <GoonPhysics/overlap.h>
#include <GoonWorld/interfaces/IDraw.hpp>

namespace GoonWorld
{
    class BoxColliderComponent : public Component, public IDraw
    {
    public:
        BoxColliderComponent(geRectangle *rect, Point offset);
        ~BoxColliderComponent();
        void AddOverlapFunction(int overlapType, OverlapFunc func);
        void OnEnabled() override;
        void OnDisabled() override;
        void Draw() override;
        void Visible(bool isVisible) override;
        bool IsVisible() override;
        inline void ParentUpdate();
        inline void SetLocation(Point location)
        {
            _boxCollider->boundingBox.x = location.x;
            _boxCollider->boundingBox.y = location.y;
        }
        inline void SizeChange(Point newSize)
        {
            _boxCollider->boundingBox.w = newSize.x;
            _boxCollider->boundingBox.h = newSize.y;
        }
        inline const Point &Offset() { return _offset; }
        inline void SetBodyType(BodyTypes bodyType) { _boxCollider->bodyType = (int)bodyType; }
        inline void SetOverlapsEnabled(bool enabled) { _boxCollider->enabled = enabled; }
        inline void SetBodyType(int bodyType) { _boxCollider->bodyType = bodyType; }
        inline void SetDebug(bool debug) { _debugDraw = debug; }
        inline gpBB &BoundingBox() { return _boxCollider->boundingBox; }
        static inline void ResetBoxColliders() { _currentBoxColliders.clear(); }

    private:
        void OnComponentAdd(GameObject &parent) override;
        gpBoxCollider *_boxCollider;
        static std::vector<BoxColliderComponent *> _currentBoxColliders;
        int _boxNum;
        Point _offset;
        bool _debugDraw = false;
    };
}