#pragma once
#include <memory>
#include <vector>
#include <GoonWorld/interfaces/IUpdate.hpp>
#include <GoonWorld/interfaces/IDraw.hpp>

namespace GoonWorld
{
    class TiledLevel;

    class Level : public IUpdate, public IDraw
    {
    public:
        Level();
        Level(const char *tiledFilename);
        ~Level();
        inline TiledLevel &GetTiledLevel() { return *_tiledLevel.get(); }
        inline int DrawLayer() override { return 0; }
        void InitializeTiledMap(const char *tiledFilename);
        void Update() override;
        void Draw() override;
        void Visible(bool isVisible) override;
        bool IsVisible() override;
        inline void AddUpdateObject(IUpdate *update) { _updateObjects.push_back(update); }
        void AddDrawObject(IDraw *draw);
        void ChangeDrawObjectLayer(IDraw *draw, int newLayer);

        inline void ClearObjects()
        {
            _updateObjects.clear();
            for (auto &layer : _drawObjects)
            {
                layer.clear();
            }
            _uiDrawObjects.clear();
        }

    private:
        std::unique_ptr<TiledLevel> _tiledLevel;

        std::vector<IUpdate *> _updateObjects;
        std::vector<std::vector<IDraw *>> _drawObjects;
        std::vector<IDraw *> _uiDrawObjects;
    };
}
