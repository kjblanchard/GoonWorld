#include <map>
#include <GoonEngine/SdlSurface.h>
#include <GoonWorld/core/Content.hpp>

using namespace GoonEngine;

static std::unordered_map<const char *, void *> _loadedContent;

void *Content::LoadContent(ContentTypes contentType, const char *filename)
{
    auto iter = _loadedContent.find(filename);

    if (iter != _loadedContent.end())
        return iter->second;
    switch (contentType)
    {
        {
        case ContentTypes::Surface:
            auto load = LoadSurfaceFromFile(filename);
            return load ? _loadedContent[filename] = load : nullptr;
        }
    default:
        break;
    }
    return nullptr;
}
template <typename T>
T *Content::GetLoadedContentOfType(const char *filename)
{
    return (T *)GetLoadedContent(filename);
}
void *Content::GetLoadedContent(const char *filename)
{
    auto iter = _loadedContent.find(filename);
    return iter != _loadedContent.end() ? iter->second : nullptr;
}