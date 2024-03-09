#include <GoonWorld/gameobjects/ItemBrick.hpp>
#include <GoonEngine/Sound.h>
#include <GoonWorld/core/Content.hpp>
#include <GoonWorld/components/RigidbodyComponent.hpp>
#include <GoonWorld/components/DebugDrawComponent.hpp>
#include <GoonWorld/gameobjects/Mushroom.hpp>

using namespace GoonWorld;

static Sfx *bumpSound;

ItemBrick::ItemBrick(TiledMap::TiledObject &object)
{
    for (auto &prop : object.Properties)
    {
        _contents = prop.ValueInt;
    }
    _location = Point{object.X, object.Y};
    auto bodyRect = SDL_Rect{object.X, object.Y, object.Width, object.Height};
    // _debugDrawComponent = new DebugDrawComponent(Point{object.Width, object.Height});
    _rigidbodyComponent = new RigidbodyComponent(&bodyRect);
    _rigidbodyComponent->SetBodyType(BodyTypes::ItemBrick);
    _rigidbodyComponent->SetStaticBody(true);
    _rigidbodyComponent->GravityEnabled(false);
    bumpSound = (Sfx *)Content::LoadContent(ContentTypes::Sfx, "bump");
    // AddComponent({_debugDrawComponent, _rigidbodyComponent});
    AddComponent({_rigidbodyComponent});
}
void ItemBrick::Update()
{
}
void ItemBrick::TakeDamage()
{
    switch (_contents)
    {
    case (int)ItemBrickContents::Empty:
        gsPlaySfxOneShot(bumpSound, 1.0f);
        /* code */
        break;
    case (int)ItemBrickContents::Mushroom:
    {
        auto loc = SDL_Rect{_location.x, _location.y - 32, 32, 32};
        auto shroom = new Mushroom(&loc);
        shroom->Push(true);
        _contents = 0;
        break;
    }

    default:
        break;
    }
}