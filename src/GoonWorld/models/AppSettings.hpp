#pragma once
#include <string>
#include <vector>
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
            std::string Title;
            std::string Name;
            float LoopStart;
            float LoopEnd;
        };
        struct SoundConfig
        {
            float MusicVolume;
            std::vector<Music> Music;
        };
        struct KeyboardConfig
        {
            std::vector<std::vector<std::vector<int>>> PlayerButtonKeyVector;
        };
        struct ControllerConfig
        {
            std::vector<std::vector<int>> PlayerButtonVector;
        };
        WindowConfig WindowConfig;
        GraphicsConfig GraphicsConfig;
        SoundConfig SoundConfigs;
        KeyboardConfig KeyboardConfig;
        ControllerConfig ControllerConfig;
    };

};