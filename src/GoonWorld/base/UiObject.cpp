#include <GoonWorld/core/Game.hpp>
#include <GoonWorld/base/UiObject.hpp>
using namespace GoonWorld;

UiObject::UiObject()
    : _enabled(true), _isVisible(true)
{
}
UiObject::~UiObject() {}

void UiObject::Enabled(bool isEnabled)
{
    if (_enabled == isEnabled)
        return;
    _enabled = isEnabled;
    if (_enabled)
        OnEnabled();
    else
        OnDisabled();
}
