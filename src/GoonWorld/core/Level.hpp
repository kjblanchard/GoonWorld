#pragma once
#include <memory>
#include <vector>
#include <GoonWorld/interfaces/IUpdate.hpp>
#include <GoonWorld/interfaces/IDraw.hpp>

namespace GoonWorld
{
    class TiledLevel;
    class Panel;

    class Level : public IUpdate, public IDraw
    {
    public:
        Level();
        Level(const char *tiledFilename);
        ~Level();
        inline TiledLevel &GetTiledLevel() { return *_tiledLevel.get(); }
        inline int DrawLayer() override { return 0; }
        inline void AddUpdateObject(IUpdate *update) { _updateObjects.push_back(update); }
        void InitializeTiledMap(const char *tiledFilename);
        void Update() override;
        void Draw() override;
        void Visible(bool isVisible) override;
        bool IsVisible() override;
        void AddDrawObject(IDraw *draw);
        void AddUiPanel(Panel *update);
        void ChangeDrawObjectLayer(IDraw *draw, int newLayer);
        void ClearObjects();

    private:
        std::unique_ptr<TiledLevel> _tiledLevel;
        std::vector<std::unique_ptr<Panel>> _uiPanels;

        std::vector<IUpdate *> _updateObjects;
        std::vector<std::vector<IDraw *>> _drawObjects;
    };
}
