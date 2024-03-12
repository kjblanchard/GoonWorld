#include <GoonWorld/gnpch.hpp>
#include <GoonEngine/SdlSurface.h>
#include <GoonWorld/core/Content.hpp>
#include <GoonEngine/Sound.h>

using namespace GoonWorld;

static std::unordered_map<std::string, std::pair<ContentTypes, void *>> _loadedContent;

void *Content::LoadContent(ContentTypes contentType, const char *filename)
{
    auto iter = _loadedContent.find(filename);

    if (iter != _loadedContent.end())
        return iter->second.second;
    void *loadedContent;
    switch (contentType)
    {
    case ContentTypes::Surface:
        loadedContent = LoadSurfaceFromFile(filename);
        break;
    case ContentTypes::Texture:
        loadedContent = CreateTextureFromFile(filename);
        break;
    case ContentTypes::Sfx:
        loadedContent = gsLoadSfxHelper(("assets/audio/" + std::string(filename) + ".ogg").c_str());
        gsLoadSfx((Sfx *)loadedContent);
        break;
    default:
        break;
    }
    LogInfo("Content count is %d", _loadedContent.size());
    if (!loadedContent)
    {
        LogWarn("Couldn't load content %s", filename);
        return nullptr;
    }
    _loadedContent[filename] = {contentType, (void *)loadedContent};
    return loadedContent;
}
template <typename T>
T *Content::GetLoadedContentOfType(const char *filename)
{
    return (T *)GetLoadedContent(filename);
}
void *Content::GetLoadedContent(const char *filename)
{
    auto iter = _loadedContent.find(filename);
    return iter != _loadedContent.end() ? iter->second.second : nullptr;
}

void Content::ClearContent()
{
    for (auto &[key, value] : _loadedContent)
    {
        switch (value.first)
        {
        case ContentTypes::Sfx:
            gsUnloadSfx((Sfx *)value.second);
            break;
        case ContentTypes::Surface:
            DestroySurface((SDL_Surface *)value.second);
            break;
        case ContentTypes::Texture:
            DestroyTexture((SDL_Texture *)value.second);
            break;
        default:
            break;
        }
    }
}