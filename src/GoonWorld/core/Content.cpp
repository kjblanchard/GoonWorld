#include <GoonWorld/gnpch.hpp>
#include <GoonEngine/SdlSurface.h>
#include <GoonWorld/core/Content.hpp>
#include <GoonWorld/content/Text.hpp>
#include <SupergoonSound/include/sound.h>

using namespace GoonWorld;

static std::unordered_map<std::string, std::pair<ContentTypes, void *>> _loadedContent;
static std::unordered_map<std::string, ILoadContent *> _loadedContents;
unsigned int Content::_currentId = 0;

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
            DestroySurface((SDL_Surface *)value.second);
            break;
        case ContentTypes::Texture:
            DestroyTexture((SDL_Texture *)value.second);
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

unsigned int Content::AddContent(ILoadContent *content)
{
    _loadedContents[content->GetContentName()] = content;
    return _currentId++;
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