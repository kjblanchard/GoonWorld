#include <GoonEngine/gnpch.h>
#include <GoonEngine/ecs/components/tagComponent.h>

TagComponent *gnTagComponentNew()
{
    TagComponent *tagComponent = calloc(1, sizeof(*tagComponent));
    return tagComponent;
}
int gnTagComponentAddTag(TagComponent *component, const char *tag)
{
    component->tags = realloc(component->tags, sizeof(char *) * component->tagCount + 1);
    component->tags[component->tagCount++] = strdup(tag);
    return true;
}

int gnTagComponentHasTag(TagComponent *component, const char *tag)
{
    for (int i = 0; i < component->tagCount; i++)
    {
        if(strcmp(component->tags[i], tag) == 0)
        {
            return true;
        }
    }
    return false;
}
const char **gnTagComponentGetTags(TagComponent *component)
{
    return component->tags;
}