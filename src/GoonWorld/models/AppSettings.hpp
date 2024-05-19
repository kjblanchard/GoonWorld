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
        struct MiscConfig
        {
            bool SkipLogos;
        };

        struct WindowConfig
        {
            Point WindowSize;
            Point WorldSize;
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
            bool Muted;
            float MusicVolume;
            float SfxVolume;
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
            bool SolidDebug, RigidBodyDebug;
            std::string InitialLevel, LogLevel;
        };
        struct PlayerConfig
        {
            int InitialJumpVelocity, FrameJumpAcceleration, InitialMoveVelocity, WalkSpeedBoost, RunSpeedBoost, MaxWalkSpeed, MaxRunSpeed;
            int RigidBodyOffsetX, RigidBodyOffsetY, RigidBodyOffsetW, RigidBodyOffsetH, JumpColliderOffsetX, JumpColliderOffsetY, JumpColliderOffsetW, JumpColliderOffsetH;
            float MaxJumpTime;
        };
        struct Animation
        {
            std::string Default;
            std::vector<AnimationConfig> Animations;
        };
        MiscConfig MiscConfig;
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

}
