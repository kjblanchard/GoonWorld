#pragma once
#include <vector>
#include <string>
#include <nlohmann/json_fwd.hpp>

namespace GoonWorld
{
    class AsepriteDocument
    {
    public:
        struct Size
        {
            int w, h;
        };

        struct SourceSize
        {
            int w, h;
        };

        struct SpriteSourceSize
        {
            int x, y, w, h;
        };
        struct Layer
        {
            std::string name, blendMode;
            int opacity;
        };
        struct Frame2
        {
            int x, y, w, h;
        };

        struct FrameTag
        {
            std::string name, direction, color;
            int from, to;
        };
        struct Frame
        {
            std::string filename;
            Frame2 frame;
            bool rotated, trimmed;
            SpriteSourceSize spriteSourceSize;
            SourceSize sourceSize;
            int duration;
        };
        struct Meta
        {
            std::string app, version, image, format, scale;
            Size size;
            std::vector<FrameTag> frameTags;
            std::vector<Layer> layers;
        };
        static AsepriteDocument from_json(nlohmann::json &json);

        std::vector<Frame> frames;
        Meta meta;
    };
}
