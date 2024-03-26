#include <GoonWorld/ui/Panel.hpp>
#include <GoonWorld/content/Image.hpp>
using namespace GoonWorld;

void Panel::AddImage(Image *image)
{
    AddUIDrawObject(image);
    AddUIUpdateObject(image);
}

void Panel::Update()
{
    for (auto update : UIUpdateObjects)
    {
        update->Update();
    }
}

void Panel::Draw()
{
    for (auto draw : UIDrawObjects)
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
