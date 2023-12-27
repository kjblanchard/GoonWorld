#pragma once

typedef struct TagComponent
{
    const char** tags;
    int tagCount;
} TagComponent;

TagComponent* gnTagComponentNew();
int gnTagComponentAddTag(TagComponent* component, const char* tag);
int gnTagComponentHasTag(TagComponent* component, const char* tag);