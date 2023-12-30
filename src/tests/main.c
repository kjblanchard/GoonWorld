#include <GoonEngine/gnpch.h>
#include <GoonEngine/test.h>
#include <GoonEngine/SdlWindow.h>
#include <GoonEngine/SdlSurface.h>
#include <GoonEngine/Sound.h>
#include <SDL2/SDL.h>
// ECS testing
#include <GoonEngine/ecs/context.h>
#include <GoonEngine/ecs/entity.h>
#include <GoonEngine/ecs/component.h>
#include <GoonEngine/ecs/components/tagComponent.h>
#include <GoonEngine/ecs/components/locationComponent.h>
#include <GoonEngine/ecs/components/scriptComponent.h>

#define TAG_COMPONENT 1
#define LOCATION_COMPONENT 2
#define SCRIPT_COMPONENT 3

void TagSystem(geContext *context, int type, void *data)
{
    Component **components = geContextGetComponentArrayByType(context, type);
    int componentCount = geContextGetComponentArrayCountByType(context, type);

    for (int i = 0; i < componentCount; i++)
    {
        Component *component = components[i];
        TagComponent *tagComp = (TagComponent *)component->Data;
        if (!tagComp)
            continue;
        for (int j = 0; j < tagComp->tagCount; j++)
        {
            printf("Component of type %d and parent %d has tag %s\n", component->Type, component->Parent->Id, tagComp->tags[j]);
        }
    }
}
void UpdateSystem(geContext *context, int type, void *data)
{
    Component **components = geContextGetComponentArrayByType(context, type);
    int componentCount = geContextGetComponentArrayCountByType(context, type);

    for (int i = 0; i < componentCount; i++)
    {
        Component *component = components[i];
        ScriptComponent *scriptComp = (ScriptComponent *)component->Data;
        if (!scriptComp)
            continue;
        scriptComp->UpdateFunc(NULL);
    }
}

void UpdateFunc(void* data)
{
    printf("Hello from update func!!\n");
}

void LocationSystem(geContext *context, int type, void *data)
{
    Component **components = context->ComponentArrays[type];
    int componentCount = context->ComponentCounts[type];

    for (int i = 0; i < componentCount; i++)
    {
        Component *component = components[i];
        LocationComponent *locComponent = (LocationComponent *)component->Data;
        if (!locComponent)
            continue;
        printf("Component of type %d and parent %d has location %d:%d\n", component->Type, component->Parent->Id, locComponent->x, locComponent->y);
    }
}

int main(int argc, char const *argv[])
{

    GnInitializeEngine();
    CreateWindowAndRenderer(300, 400, "Hello");
    SDL_Surface *purple = LoadSurfaceFromFile("assets/img/purple.png");
    SDL_Surface *atlas = LoadTextureAtlas(300, 400);
    SDL_Rect purpRect = {0, 0, 64, 64};
    SDL_Rect dstRect = {0, 0, 64, 64};
    // SDL_BlitSurface(purple, &purpRect, atlas, &dstRect);
    BlitSurface(purple, &purpRect, atlas, &dstRect);
    SDL_Texture *tex = CreateTextureFromSurface(atlas);
    SDL_Rect screenRect = {0, 0, 64, 64};
    SetBackgroundAtlas(tex, &screenRect);
    struct Bgm *mainBgm = BgmLoad("test.ogg", 20.397, 43.08);
    BgmPlay(mainBgm, 1.0);

    geContext *context = geContextNew();
    Entity *entity = geContextEntityNew(context);
    TagComponent *tagComponent = gnTagComponentNew();
    gnTagComponentAddTag(tagComponent, "Hello");
    Component *tagComp = geContextComponentNew(context, TAG_COMPONENT, tagComponent);
    geEntityAddComponent(entity, tagComp);
    LocationComponent *locComponent = gnLocationComponentNew();
    locComponent->x = 20;
    locComponent->y = 40;
    Component *locComp = geContextComponentNew(context, LOCATION_COMPONENT, locComponent);
    ScriptComponent* scriptComponent = gnScriptComponentNew();
    scriptComponent->UpdateFunc = UpdateFunc;
    Component* scriptComp = geContextComponentNew(context, SCRIPT_COMPONENT, scriptComponent );
    geEntityAddComponent(entity, locComp);

    Entity *entity2 = geContextEntityNew(context);
    TagComponent *tagComponent2 = gnTagComponentNew();
    gnTagComponentAddTag(tagComponent2, "Hello2");
    Component *tagComp2 = geContextComponentNew(context, TAG_COMPONENT, tagComponent2);
    geEntityAddComponent(entity2, tagComp2);


    geContextSystemNew(context, TagSystem, TAG_COMPONENT);
    geContextSystemNew(context, LocationSystem, LOCATION_COMPONENT);
    geContextSystemNew(context, UpdateSystem, SCRIPT_COMPONENT);
    bool ent1TagBool = geEntityHasComponent(entity, TAG_COMPONENT);
    bool ent1LocBool = geEntityHasComponent(entity, LOCATION_COMPONENT);
    Component* ent1TagComp = geEntityGetComponent(entity, TAG_COMPONENT);
    Component* ent1LocComp = geEntityGetComponent(entity, LOCATION_COMPONENT);
    bool ent2TagBool = geEntityHasComponent(entity2, TAG_COMPONENT);
    bool ent2LocBool = geEntityHasComponent(entity2, LOCATION_COMPONENT);
    Component* ent2TagComp = geEntityGetComponent(entity2, TAG_COMPONENT);
    Component* ent2LocComp = geEntityGetComponent(entity2, LOCATION_COMPONENT);

    geContextUpdate(context, NULL);
    // Entity* entity2 =  geContextEntityNew(context);

    Play();
    return 0;
}
