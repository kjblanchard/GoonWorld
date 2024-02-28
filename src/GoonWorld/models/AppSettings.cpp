#include <fstream>
#include <json/json.hpp>
#include <GoonWorld/models/AppSettings.hpp>
using json = nlohmann::json;
using namespace GoonWorld;

AppSettings::AppSettings(const char *filepath)
{
    std::ifstream f("assets/config/appsettings.json");
    json data = json::parse(f);
    auto width = data["windowConfig"]["windowSize"]["x"].get<float>();
    WindowConfig.WindowSize.x = data["windowConfig"]["windowSize"]["x"].get<float>();
    WindowConfig.WindowSize.y = data["windowConfig"]["windowSize"]["y"].get<float>();
    WindowConfig.Title = data["windowConfig"]["title"].get<std::string>();
    auto soundConfig = data["soundConfig"];
    SoundConfigs.MusicVolume = soundConfig["musicVolume"];
    for (auto &song : soundConfig["musicToLoad"])
    {
        Music music;
        music.Name = song["name"];
        music.Title = song["title"];
        music.LoopStart = song["startLoop"];
        music.LoopEnd = song["endLoop"];
        SoundConfigs.Music.push_back(music);
    }
    auto &keyboardConfigJson = data["keyboardConfig"];
    auto &controllerConfigJson = data["controllerConfig"];
    KeyboardConfig.PlayerButtonKeyVector = keyboardConfigJson.get<decltype(KeyboardConfig.PlayerButtonKeyVector)>();
    ControllerConfig.PlayerButtonVector = controllerConfigJson.get<decltype(ControllerConfig.PlayerButtonVector)>();
    auto &playerConfigJson = data["playerConfig"];
    PlayerConfigs.MoveSpeed = playerConfigJson["moveSpeed"];
    PlayerConfigs.RunSpeed = playerConfigJson["runSpeed"];
    PlayerConfigs.FrameJumpAcceleration = playerConfigJson["frameJumpAcceleration"];
    PlayerConfigs.InitialJumpVelocity = playerConfigJson["initialJumpVelocity"];
    PlayerConfigs.MaxJumpTime = playerConfigJson["maxJumpTime"];
    PlayerConfigs.InitialMoveVelocity = playerConfigJson["initialMoveVelocity"];
    auto &animationConfigJson = data["animationConfig"];
    for (auto &[key, value] : animationConfigJson.items())
    {
        Animation animation;
        animation.Default = value["default"];
        for (auto &anim : value["animations"])
        {
            struct AnimationConfig animConfig;
            animConfig.Name = anim["name"];
            animConfig.Looping = anim["looping"];
            animation.Animations.push_back(animConfig);
        }
        AnimationConfig[key] = animation;
    }
}