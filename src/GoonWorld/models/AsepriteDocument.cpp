#include <nlohmann/json.hpp>
#include <GoonWorld/models/AsepriteDocument.hpp>
using json = nlohmann::json;
using namespace GoonWorld;

AsepriteDocument AsepriteDocument::from_json(json &j)
{
    AsepriteDocument document;

    // Parse frames array
    for (const auto &frameJson : j["frames"])
    {
        Frame frame;
        frame.filename = frameJson["filename"];
        frame.frame = {
            frameJson["frame"]["x"],
            frameJson["frame"]["y"],
            frameJson["frame"]["w"],
            frameJson["frame"]["h"]};
        frame.rotated = frameJson["rotated"];
        frame.trimmed = frameJson["trimmed"];
        frame.spriteSourceSize = {
            frameJson["spriteSourceSize"]["x"],
            frameJson["spriteSourceSize"]["y"],
            frameJson["spriteSourceSize"]["w"],
            frameJson["spriteSourceSize"]["h"]};
        frame.sourceSize = {
            frameJson["sourceSize"]["w"],
            frameJson["sourceSize"]["h"]};
        frame.duration = frameJson["duration"];

        document.frames.push_back(frame);
    }

    // Parse meta information
    document.meta.app = j["meta"]["app"];
    document.meta.version = j["meta"]["version"];
    document.meta.image = j["meta"]["image"];
    document.meta.format = j["meta"]["format"];
    document.meta.scale = j["meta"]["scale"];
    document.meta.size = {
        j["meta"]["size"]["w"],
        j["meta"]["size"]["h"]};

    // Parse frameTags array
    for (const auto &tagJson : j["meta"]["frameTags"])
    {
        FrameTag tag;
        tag.name = tagJson["name"];
        tag.direction = tagJson["direction"];
        tag.color = tagJson["color"];
        tag.from = tagJson["from"];
        tag.to = tagJson["to"];

        document.meta.frameTags.push_back(tag);
    }

    // Parse layers array
    for (const auto &layerJson : j["meta"]["layers"])
    {
        Layer layer;
        layer.name = layerJson["name"];
        layer.opacity = layerJson["opacity"];
        layer.blendMode = layerJson["blendMode"];

        document.meta.layers.push_back(layer);
    }

    return document;
}