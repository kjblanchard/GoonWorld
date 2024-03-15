#include <GoonWorld/gameobjects/ItemBrick.hpp>
#include <GoonWorld/core/Content.hpp>
#include <GoonWorld/components/RigidbodyComponent.hpp>
#include <GoonWorld/components/DebugDrawComponent.hpp>
#include <GoonWorld/gameobjects/Mushroom.hpp>
#include <GoonWorld/core/Sound.hpp>
#include <GoonEngine/rectangle.h>

using namespace GoonWorld;

const char* bumpSound = "bump";
const char* powerupSpawnSound = "powerup";

ItemBrick::ItemBrick(TiledMap::TiledObject &object)
{
    for (auto &prop : object.Properties)
    {
        _contents = prop.ValueInt;
    }
    _location = Point{object.X, object.Y};
    auto bodyRect = geRectangle{object.X, object.Y, object.Width, object.Height};
    _debugDrawComponent = new DebugDrawComponent(Point{object.Width, object.Height});
    _rigidbodyComponent = new RigidbodyComponent(&bodyRect);
    _rigidbodyComponent->SetBodyType(BodyTypes::ItemBrick);
    _rigidbodyComponent->SetStaticBody(true);
    _rigidbodyComponent->GravityEnabled(false);
    GetGameSound().LoadSfx(bumpSound);
    GetGameSound().LoadSfx(powerupSpawnSound);
    AddComponent({_rigidbodyComponent, _debugDrawComponent});
    _debugDrawComponent->Enabled(false);
}
void ItemBrick::Update()
{
}
void ItemBrick::TakeDamage()
{
    switch (_contents)
    {
    case (int)ItemBrickContents::Empty:
        GetGameSound().PlaySfx(bumpSound);
        /* code */
        break;
    case (int)ItemBrickContents::Mushroom:
    {
        auto loc = geRectangle{_location.x, _location.y - 16, 16, 16};
        auto shroom = new Mushroom(&loc);
        shroom->Push(true);
        _contents = 0;
        GetGameSound().PlaySfx(powerupSpawnSound);
        break;
    }

    default:
        break;
    }
}