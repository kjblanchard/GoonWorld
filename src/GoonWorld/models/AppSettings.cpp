#include <GoonWorld/models/AppSettings.hpp>
using json = nlohmann::json;
using namespace GoonWorld;

AppSettings::AppSettings(const char *filepath)
{
    std::ifstream f("assets/config/appsettings.json");
    json data = json::parse(f);
    // auto width = data["windowConfig"]["windowSize"]["x"].get<float>();
    WindowConfig.WindowSize.x = data["windowConfig"]["windowSize"]["x"].get<float>();
    WindowConfig.WindowSize.y = data["windowConfig"]["windowSize"]["y"].get<float>();
    WindowConfig.WorldSize.x = data["windowConfig"]["worldSize"]["x"].get<float>();
    WindowConfig.WorldSize.y = data["windowConfig"]["worldSize"]["y"].get<float>();
    WindowConfig.Title = data["windowConfig"]["title"].get<std::string>();
    auto debugConfig = data["debugConfig"];
    DebugConfig.SolidDebug = debugConfig["solidObjectDebug"];
    auto soundConfig = data["soundConfig"];
    SoundConfigs.MusicVolume = soundConfig["musicVolume"];
    SoundConfigs.Muted = soundConfig["mute"];
    SoundConfigs.SfxVolume = soundConfig["sfxVolume"];
    for (auto &song : soundConfig["musicToLoad"])
    {
        Music music;
        music.Name = song["name"];
        music.LoopStart = song["startLoop"];
        music.LoopEnd = song["endLoop"];
        SoundConfigs.Music.push_back(music);
    }
    auto &keyboardConfigJson = data["keyboardConfig"];
    auto &controllerConfigJson = data["controllerConfig"];
    KeyboardConfig.PlayerButtonKeyVector = keyboardConfigJson.get<decltype(KeyboardConfig.PlayerButtonKeyVector)>();
    ControllerConfig.PlayerButtonVector = controllerConfigJson.get<decltype(ControllerConfig.PlayerButtonVector)>();
    // Player config
    auto &playerConfigJson = data["playerConfig"];
    LoadPlayerConfigFromJson(playerConfigJson);

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

void AppSettings::LoadPlayerConfigFromJson(nlohmann::json &playerConfigJson)
{
    PlayerConfigs.WalkSpeedBoost = playerConfigJson["walkSpeedBoost"];
    PlayerConfigs.RunSpeedBoost = playerConfigJson["runSpeedBoost"];
    PlayerConfigs.MaxWalkSpeed = playerConfigJson["maxWalkSpeed"];
    PlayerConfigs.MaxRunSpeed = playerConfigJson["maxRunSpeed"];
    PlayerConfigs.FrameJumpAcceleration = playerConfigJson["frameJumpAcceleration"];
    PlayerConfigs.InitialJumpVelocity = playerConfigJson["initialJumpVelocity"];
    PlayerConfigs.MaxJumpTime = playerConfigJson["maxJumpTime"];
    PlayerConfigs.InitialMoveVelocity = playerConfigJson["initialMoveVelocity"];
}