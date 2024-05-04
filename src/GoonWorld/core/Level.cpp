#include <GoonWorld/gnpch.hpp>
#include <GoonWorld/core/Game.hpp>
#include <GoonWorld/core/Level.hpp>
#include <GoonWorld/tiled/TiledLevel.hpp>
#include <GoonWorld/interfaces/IEnable.hpp>
#include <GoonWorld/ui/Panel.hpp>
#include <GoonWorld/base/GameObject.hpp>
#include <GoonPhysics/scene.h>
#include <GoonEngine/debug.h>
#include <GoonWorld/content/Bgm.hpp>
#include <GoonEngine/game.h>
using namespace GoonWorld;

extern std::map<std::string, std::function<GameObject *(TiledMap::TiledObject &)>> GameSpawnMap;
Level::Level()
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
    Unload();
}

void Level::Start()
{
    gpSceneSetEnabled(true);
    if (_bgm)
    {
        _bgm->Play(-1, _bgmVolume);
    }
}

void Level::Load()
{
    if (_isLoaded)
        return;
    if (_tiledLevel)
    {
        auto [bgmName, bgmStart, bgmEnd, volume] = _tiledLevel->GetBgmData();
        _bgm = Bgm::BgmFactory(bgmName, bgmStart, bgmEnd);
        _bgmVolume = volume;
        // LoadTiledGameObjects();
    }

    gpSceneSetEnabled(false);
    _isLoaded = true;
}

void Level::Unload()
{
    ClearObjects();
    _uiPanels.clear();
    _drawObjects.clear();
}

void Level::RestartLevel()
{
    if (_tiledLevel)
    {
        InitializePhysics();
        auto gravity = _tiledLevel->GetGravity();
        gpSceneSetGravity(_scene, gravity.y);
        gpSceneSetFriction(_scene, gravity.x);
        ClearObjects();
        // _gameObjects.clear();
        LoadTiledGameObjects();
        _tiledLevel->RestartLevel();
    }
}

void Level::InitializePhysics()
{
    if (_scene)
        gpSceneFree(_scene);
    _scene = gpInitScene();
    geSetCurrentScene(_scene);
    gpSceneSetEnabled(false);
}

void Level::LoadTiledGameObjects()
{
    for (auto &object : _tiledLevel->GetAllObjects())
    {
        auto iter = GameSpawnMap.find(object.ObjectType);
        if (iter == GameSpawnMap.end())
            continue;
        auto sharedGo = std::shared_ptr<GameObject>((*iter).second(object));
        // _gameObjects.push_back(std::shared_ptr<GameObject>((*iter).second(object)));
        _gameObjects.push_back(sharedGo);
    }
}

void Level::ClearObjects()
{
    _updateObjects.clear();
    for (auto &layer : _drawObjects)
    {
        layer.clear();
    }
    _uiPanels.clear();
    _gameObjects.clear();
}

void Level::InitializeTiledMap(const char *tiledFilename)
{
    _tiledLevel = std::make_unique<TiledLevel>(tiledFilename);
}

void Level::AddUiPanel(Panel *update) { _uiPanels.push_back(std::unique_ptr<Panel>(update)); }

void Level::Update()
{
    GameObject::UpdateTimers();
    for (auto &object : _updateObjects)
    {
        auto updateEnable = dynamic_cast<IEnable *>(object);
        if (updateEnable && !updateEnable->IsEnabled())
            continue;
        object->Update();
    }

    for (auto &object : _uiPanels)
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

    for (auto &object : _uiPanels)
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