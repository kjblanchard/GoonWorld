#include <GoonWorld/gnpch.hpp>
#include <GoonEngine/SdlSurface.h>
#include <GoonWorld/core/Content.hpp>
#include <GoonWorld/content/Text.hpp>
#include <SupergoonSound/include/sound.h>

using namespace GoonWorld;

static std::unordered_map<std::string, std::pair<ContentTypes, void *>> _loadedContent;
static std::unordered_map<std::string, ILoadContent *> _loadedContents;
// static std::vector<void *> _loadedPixelData;

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
        // _loadedPixelData.push_back(pixelData);
        break;
    case ContentTypes::Texture:
        loadedContent = CreateTextureFromFile(filename);
        break;
    case ContentTypes::Bgm:
        loadedContent = gsLoadBgm((filename));
        break;
    case ContentTypes::Sfx:

        loadedContent = gsLoadSfxHelper(filename);
        gsLoadSfx((gsSfx *)loadedContent);
        break;
    default:
        break;
    }
    if (!loadedContent)
    {
        LogWarn("Couldn't load content %s", filename);
        return nullptr;
    }
    _loadedContent[filename] = {contentType, (void *)loadedContent};
    return loadedContent;
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
            gsUnloadSfx((gsSfx *)value.second);
            break;
        case ContentTypes::Surface:
            DestroySurface((SDL_Surface *)value.second);
            break;
        case ContentTypes::Texture:
            DestroyTexture((SDL_Texture *)value.second);
            break;
        case ContentTypes::Bgm:
            free((gsBgm *)value.second);
            break;
        default:
            break;
        }
    }
}

void Content::CreateContent(ILoadContent *content)
{
    auto iter = _loadedContents.find(content->GetContentName());
    if (iter != _loadedContents.end())
        return;
    _loadedContents[content->GetContentName()] = content;
}

void Content::LoadAllContents()
{
    for (auto &[key, value] : _loadedContents)
    {
        value->Load();
    }
}

ILoadContent *Content::GetContent(std::string &name)
{
    auto iter = _loadedContents.find(name);
    if (iter != _loadedContents.end())
        return iter->second;
    return nullptr;
}