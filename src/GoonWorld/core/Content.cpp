#include <GoonWorld/gnpch.hpp>
#include <GoonEngine/SdlSurface.h>
#include <GoonWorld/core/Content.hpp>
#include <GoonWorld/content/Text.hpp>
#include <SupergoonSound/include/sound.h>

using namespace GoonWorld;

static std::unordered_map<std::string, std::pair<ContentTypes, void *>> _loadedContent;
static std::unordered_map<std::string, ILoadContent *> _loadedContents;

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
        loadedContent = geCreateTextureFromFile(filename);
        break;
    default:
        loadedContent = nullptr;
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
        case ContentTypes::Surface:
            geDestroySurface((SDL_Surface *)value.second);
            break;
        case ContentTypes::Texture:
            geDestroyTexture((SDL_Texture *)value.second);
            break;
        default:
            break;
        }
    }
    for (auto &[key, value] : _loadedContents)
    {
        if (value)
        {
            delete (value);
        }
    }
    _loadedContent.clear();
    _loadedContents.clear();
}

void Content::AddContent(ILoadContent *content)
{
    _loadedContents[content->GetContentName()] = content;
}

void Content::LoadAllContent()
{
    for (auto &[key, value] : _loadedContents)
    {
        if (!value->IsLoaded())
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

void Content::RemoveContent(std::string &name)
{
    auto it = _loadedContents.find(name);

    // If the element is found, erase it
    if (it != _loadedContents.end())
    {
        _loadedContents.erase(it);
    }
}

void Content::RemoveContent(ILoadContent *content)
{
    auto name = content->GetContentName();
    RemoveContent(name);
}