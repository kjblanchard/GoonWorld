#include <GoonWorld/gnpch.hpp>
#include <GoonEngine/SdlSurface.h>
#include <GoonWorld/core/Content.hpp>

using namespace GoonWorld;

static std::unordered_map<const char *, void *> _loadedContent;

void *Content::LoadContent(ContentTypes contentType, const char *filename)
{
    auto iter = _loadedContent.find(filename);

    if (iter != _loadedContent.end())
        return iter->second;
    void *loadedContent;
    switch (contentType)
    {
    case ContentTypes::Surface:
        loadedContent = LoadSurfaceFromFile(filename);
        return loadedContent ? _loadedContent[filename] = loadedContent : nullptr;
    case ContentTypes::Texture:
        loadedContent = CreateTextureFromFile(filename);
        return loadedContent ? _loadedContent[filename] = loadedContent : nullptr;
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