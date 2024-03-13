#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <GoonEngine/point.h>

namespace GoonWorld
{
    class AppSettings
    {
    public:
        AppSettings(const char *filepath);

        struct WindowConfig
        {
            Point WindowSize;
            std::string Title;
        };
        struct GraphicsConfig
        {
            bool Vsync;
            int Fps;
        };
        struct Music
        {
            std::string Name;
            float LoopStart;
            float LoopEnd;
        };
        struct SoundConfig
        {
            float MusicVolume;
            std::vector<struct Music> Music;
        };
        struct KeyboardConfig
        {
            std::vector<std::vector<std::vector<int>>> PlayerButtonKeyVector;
        };
        struct ControllerConfig
        {
            std::vector<std::vector<int>> PlayerButtonVector;
        };
        struct AnimationConfig
        {
            std::string Name;
            bool Looping;
        };
        struct DebugConfig
        {
            bool SolidDebug;
        };
        struct PlayerConfig
        {
            float InitialJumpVelocity, FrameJumpAcceleration, InitialMoveVelocity, WalkSpeedBoost, RunSpeedBoost, MaxWalkSpeed, MaxRunSpeed, MaxJumpTime;
        };
        struct Animation
        {
            std::string Default;
            std::vector<AnimationConfig> Animations;
        };
        WindowConfig WindowConfig;
        GraphicsConfig GraphicsConfig;
        SoundConfig SoundConfigs;
        KeyboardConfig KeyboardConfig;
        ControllerConfig ControllerConfig;
        PlayerConfig PlayerConfigs;
        DebugConfig DebugConfig;
        std::unordered_map<std::string, Animation> AnimationConfig;

    private:
        void LoadPlayerConfigFromJson(nlohmann::json &playerConfigJson);
    };

};