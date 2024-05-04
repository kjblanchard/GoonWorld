#pragma once
#include <memory>
#include <vector>
#include <GoonWorld/interfaces/IStart.hpp>
#include <GoonWorld/interfaces/IUpdate.hpp>
#include <GoonWorld/interfaces/IDraw.hpp>
#include <GoonWorld/interfaces/ILoad.hpp>

namespace GoonWorld
{
    class TiledLevel;
    class Panel;
    class Bgm;

    class Level : public IUpdate, public IDraw, public IStart, public ILoad
    {
    public:
        Level();
        Level(const char *tiledFilename);
        ~Level();
        void Start() override;
        inline TiledLevel &GetTiledLevel() { return *_tiledLevel.get(); }
        inline int DrawLayer() override { return 0; }
        inline void AddUpdateObject(IUpdate *update) { _updateObjects.push_back(update); }
        void RestartLevel();
        void InitializeTiledMap(const char *tiledFilename);
        void Update() override;
        void Draw() override;
        void Visible(bool isVisible) override;
        bool IsVisible() override;
        void AddDrawObject(IDraw *draw);
        void AddUiPanel(Panel *update);
        void ChangeDrawObjectLayer(IDraw *draw, int newLayer);
        void ClearObjects();
        inline void SetBgm(Bgm *bgm) { _bgm = bgm; }
        inline void SetBgmVolume(float volume) { _bgmVolume = volume; }
        void Load() override;
        void Unload() override;
        void LoadTiledGameObjects();
        inline bool IsLoaded() override { return _isLoaded; }
        void InitializePhysics();

    private:
        std::unique_ptr<TiledLevel> _tiledLevel;

        std::vector<std::shared_ptr<GameObject>> _gameObjects;

        std::vector<std::unique_ptr<Panel>> _uiPanels;
        std::vector<IUpdate *> _updateObjects;
        std::vector<std::vector<IDraw *>> _drawObjects;
        bool _isLoaded;
        Bgm *_bgm;
        gpScene *_scene;
        float _bgmVolume;
    };
}
