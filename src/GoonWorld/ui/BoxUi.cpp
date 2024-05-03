#include <GoonWorld/ui/BoxUi.hpp>
#include <GoonEngine/SdlSurface.h>
using namespace GoonWorld;

BoxUi::BoxUi(geRectangle rect)
{
    _location = rect;
    _drawColor = geColor{0, 0, 0, 255};
}

void BoxUi::Draw()
{
    geDrawDebugRect(&_location, &_drawColor);
}