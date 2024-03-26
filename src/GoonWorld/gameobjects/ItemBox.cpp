#include <GoonWorld/gameobjects/ItemBox.hpp>
#include <GoonWorld/core/Content.hpp>
#include <GoonWorld/components/RigidbodyComponent.hpp>
#include <GoonWorld/components/AnimationComponent.hpp>
#include <GoonWorld/components/DebugDrawComponent.hpp>
#include <GoonWorld/gameobjects/Mushroom.hpp>
#include <GoonWorld/gameobjects/Fireflower.hpp>
#include <GoonWorld/core/Sound.hpp>
#include <GoonEngine/rectangle.h>
#include <GoonWorld/content/Sfx.hpp>

using namespace GoonWorld;

extern const char *bumpSound;
extern const char *powerupSpawnSound;

ItemBox::ItemBox(TiledMap::TiledObject &object)
{
    for (auto &prop : object.Properties)
    {
        _contents = prop.ValueInt;
    }
    _location = Point{object.X, object.Y};
    auto bodyRect = geRectangle{object.X, object.Y, object.Width, object.Height};
    _debugDrawComponent = new DebugDrawComponent(Point{object.Width, object.Height});
    _rigidbodyComponent = new RigidbodyComponent(&bodyRect);
    _rigidbodyComponent->SetBodyType(BodyTypes::ItemBox);
    _rigidbodyComponent->SetStaticBody(true);
    _rigidbodyComponent->GravityEnabled(false);
    _animationComponent = new AnimationComponent("itembox");
    bumpSfx = Sfx::SfxFactory(bumpSound);
    powerupSpawnSfx = Sfx::SfxFactory(powerupSpawnSound);
    AddComponent({_rigidbodyComponent, _debugDrawComponent, _animationComponent});
    _debugDrawComponent->Enabled(false);
    _animationComponent->AddTransition("idle", "empty", true, &_isOpened);
    _animationComponent->ChangeLayer(1);
    auto loc = geRectangle{_location.x, _location.y, 16, 16};
    switch (_contents)
    {
    case (int)ItemBrickContents::Mushroom:
        content = new Mushroom(&loc);
        break;
    case (int)ItemBrickContents::Fireflower:
        content = new Fireflower(&loc);
    default:
        break;
    }
    content->Enabled(false);
}
void ItemBox::TakeDamage()
{
    switch (_contents)
    {
    case (int)ItemBrickContents::Empty:
        bumpSfx->Play();
        break;
    case (int)ItemBrickContents::Mushroom:
    {
        auto shroom = dynamic_cast<Mushroom *>(content);
        shroom->Push(true);
        powerupSpawnSfx->Play();
        break;
    }
    case (int)ItemBrickContents::Fireflower:
    {
        auto flower = dynamic_cast<Fireflower *>(content);
        flower->Push();
        // content->Enabled(true);
        powerupSpawnSfx->Play();
        break;
    }

    default:
        break;
    }
    _contents = 0;
    _isOpened = true;
}