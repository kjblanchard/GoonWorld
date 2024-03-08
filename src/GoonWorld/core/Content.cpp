#include <GoonWorld/gnpch.hpp>
#include <GoonEngine/SdlSurface.h>
#include <GoonWorld/core/Content.hpp>
#include <GoonEngine/Sound.h>

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
        // return loadedContent ? _loadedContent[filename] = loadedContent : nullptr;
        break;
    case ContentTypes::Texture:
        loadedContent = CreateTextureFromFile(filename);
        // return loadedContent ? _loadedContent[filename] = loadedContent : nullptr;
        break;
    case ContentTypes::Sfx:
        loadedContent = LoadSfxHelper(("assets/audio/" + std::string(filename) + ".ogg").c_str());
        break;
    default:
        break;
    }
    return loadedContent ? _loadedContent[filename] = loadedContent : nullptr;
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