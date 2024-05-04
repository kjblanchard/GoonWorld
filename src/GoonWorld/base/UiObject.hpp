#pragma once
#include <memory>
#include <list>
#include <GoonWorld/tiled/TiledMap.hpp>
#include <GoonWorld/interfaces/IUpdate.hpp>
#include <GoonWorld/interfaces/IStart.hpp>
#include <GoonWorld/interfaces/IEnable.hpp>
#include <GoonWorld/interfaces/IDraw.hpp>
#include <GoonWorld/interfaces/IReset.hpp>
#include <GoonWorld/common/TimeSpan.hpp>
#include <GoonEngine/point.h>

namespace GoonWorld
{
    class UiObject : public IUpdate, public IStart, public IEnable, public IDraw, public IReset
    {
    public:
        UiObject();
        virtual ~UiObject();
        virtual void Enabled(bool isEnabled) override;
        inline virtual void Start() override {}
        inline virtual void Update() override {}
        inline bool IsEnabled() const override { return _enabled; }
        inline virtual void OnEnabled() override {}
        inline virtual void OnDisabled() override {}
        inline virtual void Draw() override {}
        inline virtual void Reset() override {}
        inline virtual void Visible(bool isVisible) override { _isVisible = isVisible; }
        inline virtual bool IsVisible() override { return _isVisible; }

    private:
        bool _enabled;
        bool _isVisible;
    };
}