#include <GoonWorld/core/Level.hpp>
#include <GoonWorld/tiled/TiledLevel.hpp>
#include <GoonWorld/interfaces/IEnable.hpp>
#include <GoonWorld/ui/Panel.hpp>
using namespace GoonWorld;
Level::Level()
    // : _tiledLevel(nullptr), _drawObjects(4), _uiDrawObjects(4)
    : _tiledLevel(nullptr), _drawObjects(4)
{
}

Level::Level(const char *tiledFilename)
    : Level()
{
    InitializeTiledMap(tiledFilename);
}
Level::~Level()
{
    ClearObjects();
    _drawObjects.clear();
}

void Level::ClearObjects()
{
    _updateObjects.clear();
    for (auto &layer : _drawObjects)
    {
        layer.clear();
    }
    for (auto &ui : _uiPanels)
    {
        delete (ui);
    }
    _uiPanels.clear();
    // _uiDrawObjects.clear();
}

void Level::InitializeTiledMap(const char *tiledFilename)
{
    _tiledLevel = std::make_unique<TiledLevel>(tiledFilename);
}

void Level::Update()
{
    for (auto object : _updateObjects)
    {
        auto updateEnable = dynamic_cast<IEnable *>(object);
        if (updateEnable && !updateEnable->IsEnabled())
            continue;
        object->Update();
    }

    for (auto object : _uiPanels)
    {
        if (!object->IsEnabled())
            continue;
        object->Update();
    }
}
void Level::Draw()
{
    for (auto layer : _drawObjects)
    {
        for (auto object : layer)
        {
            if (object->IsVisible())
                object->Draw();
        }
    }

    for (auto object : _uiPanels)
    {
        if (!object->IsEnabled())
            continue;
        object->Draw();
    }
}
void Level::Visible(bool isVisible)
{
}
bool Level::IsVisible()
{
}
void Level::ChangeDrawObjectLayer(IDraw *draw, int newLayer)
{
    int currentLayer = draw->DrawLayer(); // Current layer of the IDraw object
    int objectIndex = -1;

    for (size_t i = 0; i < _drawObjects[currentLayer].size(); ++i)
    {

        if (_drawObjects[currentLayer][i] == draw)
        {
            objectIndex = i; // Store the current layer
            break;           // Exit the inner loop
        }
    }

    if (objectIndex != -1)
    {
        _drawObjects[newLayer].push_back(draw);
        _drawObjects[currentLayer].erase(_drawObjects[currentLayer].begin() + objectIndex);
    }
}
void Level::AddDrawObject(IDraw *draw)
{
    _drawObjects[draw->DrawLayer()].push_back(draw);
}

// void Level::AddUiDrawObject(IDraw *draw)
// {
//     _uiDrawObjects[draw->DrawLayer()].push_back(draw);
// }