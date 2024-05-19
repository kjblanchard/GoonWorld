#include <GoonWorld/ui/Panel.hpp>
#include <GoonWorld/content/Image.hpp>
#include <GoonWorld/content/Text.hpp>
#include <GoonEngine/debug.h>
using namespace GoonWorld;

Panel::~Panel()
{
    UiObjects.clear();
}

void Panel::AddText(Text *image)
{
    AddUIDrawObject(image);
    // AddUIDrawObject(image);
    // AddUIUpdateObject(image);
}
void Panel::AddImage(Image *image)
{
    AddUIDrawObject(image);
    // AddUIUpdateObject(image);
}

void Panel::Update()
{
    for (auto& update : UiObjects)
    {
        update->Update();
    }
}

void Panel::Draw()
{
    for (auto& draw : UiObjects)
    {
        draw->Draw();
    }
}

void Panel::Visible(bool isVisible)
{
}

bool Panel::IsVisible()
{
    return true;
}
bool Panel::IsEnabled() const
{
    return _enabled;
}
void Panel::Enabled(bool isEnabled)
{
    _enabled = isEnabled;

}
void Panel::OnEnabled()
{
}
void Panel::OnDisabled()
{
}
