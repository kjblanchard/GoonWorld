#include <GoonWorld/animation/Animator.hpp>
#include <GoonWorld/models/AsepriteDocument.hpp>
#include <GoonWorld/animation/Animation.hpp>
#include <GoonWorld/core/Game.hpp>
#include <GoonWorld/models/AppSettings.hpp>
#include <GoonWorld/core/Content.hpp>
#include <fstream>
#include <json/json.hpp>
using json = nlohmann::json;
using namespace GoonWorld;

std::unordered_map<std::string, Animator *> Animator::LoadedAnimators;

Animator::Animator(std::string filepath)
{
    auto fullPathNoType = "assets/img/" + filepath;
    auto fullPathJson = fullPathNoType + ".json";
    auto fullPathImg = fullPathNoType + ".png";
    std::ifstream fileStream(fullPathJson);
    auto fileJson = json::parse(fileStream);
    _loadedDocument = GoonWorld::AsepriteDocument::from_json(fileJson);
    auto config = Game::Instance()->GameSettings->AnimationConfig[filepath];
    _defaultAnimation = config.Default;
    for (auto &animation : config.Animations)
    {
        Animation anim;
        anim.Looping = animation.Looping;
        anim.Name = animation.Name;
        for (auto &transition : _loadedDocument.meta.frameTags)
        {
            if (transition.name == animation.Name)
            {
                anim.StartFrame = transition.from;
                anim.EndFrame = transition.to;
            }
        }
        anim.Image = (SDL_Texture *)Content::LoadContent(ContentTypes::Texture, fullPathImg.c_str());
        _animations.push_back(anim);
    }
}