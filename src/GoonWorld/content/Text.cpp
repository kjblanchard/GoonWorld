#include <GoonWorld/content/Text.hpp>
#include <GoonEngine/text.h>
using namespace std;
using namespace GoonWorld;

Text::Text(string stringToLoad)
: _stringToDisplay(stringToLoad)
{
    // auto thing = geCreateTextureForString(stringToLoad.c_str(), );
}

Text::~Text()
{
}

void Text::Load()
{
}

void Text::Unload(bool isVisible)
{
}