#include <GoonWorld/components/DebugDrawComponent.hpp>
#include <GoonWorld/base/GameObject.hpp>
#include <GoonEngine/SdlWindow.h>
#include <SDL2/SDL_rect.h>
#include <GoonWorld/core/Game.hpp>
using namespace GoonWorld;

DebugDrawComponent::DebugDrawComponent(Point size)
    : Component((int)ComponentTypes::DebugDrawComponent), DrawColor{255, 0, 0, 255}, Size(size), _visible(true)
{
}
void DebugDrawComponent::Draw()
{
    if (!_enabled)
        return;
    SDL_Rect dstRect{
        _parent->Location().x,
        _parent->Location().y,
        Size.x,
        Size.y};
    DrawDebugRect(&dstRect, &DrawColor);
}
void DebugDrawComponent::OnComponentAdd(GameObject &parent)
{
    Component::OnComponentAdd(parent);
    Enabled(true);
    Game::Instance()->DrawObjects.push_back(this);
}
