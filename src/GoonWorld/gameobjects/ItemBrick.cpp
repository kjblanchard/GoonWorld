#include <GoonWorld/gameobjects/ItemBrick.hpp>
#include <GoonWorld/core/Content.hpp>
#include <GoonWorld/components/RigidbodyComponent.hpp>
#include <GoonWorld/components/DebugDrawComponent.hpp>
#include <GoonWorld/gameobjects/Mushroom.hpp>
#include <GoonWorld/core/Sound.hpp>
#include <GoonWorld/content/Sfx.hpp>
#include <GoonEngine/rectangle.h>

using namespace GoonWorld;

const char *bumpSound = "bump";
const char *powerupSpawnSound = "powerup";

static Sfx *bumpSfx2 = nullptr;
static Sfx *powerupSpawnSfx2 = nullptr;

ItemBrick::ItemBrick(TiledMap::TiledObject &object)
    : _contents(0)
{
    for (auto &prop : object.Properties)
    {
        _contents = prop.ValueInt;
    }
    _location = Point{object.X, object.Y};
    _previousLocation = Point{object.X, object.Y};
    auto bodyRect = geRectangle{object.X, object.Y, object.Width, object.Height};
    _debugDrawComponent = new DebugDrawComponent(Point{object.Width, object.Height});
    _rigidbodyComponent = new RigidbodyComponent(&bodyRect);
    _rigidbodyComponent->SetBodyType(BodyTypes::ItemBrick);
    _rigidbodyComponent->SetStaticBody(true);
    _rigidbodyComponent->GravityEnabled(false);
    bumpSfx2 = Sfx::SfxFactory(bumpSound);
    powerupSpawnSfx2 = Sfx::SfxFactory(powerupSpawnSound);
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
        bumpSfx2->Play();
        /* code */
        break;
    case (int)ItemBrickContents::Mushroom:
    {
        auto loc = geRectangle{_location.x, _location.y - 16, 16, 16};
        auto shroom = new Mushroom(&loc);
        shroom->Push(true);
        _contents = 0;
        powerupSpawnSfx2->Play();
        break;
    }

    default:
        break;
    }
}