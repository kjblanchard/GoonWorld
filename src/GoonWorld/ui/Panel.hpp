#pragma once
#include <vector>
#include <GoonWorld/interfaces/IDraw.hpp>
#include <GoonWorld/interfaces/IUpdate.hpp>
#include <GoonWorld/interfaces/IEnable.hpp>

namespace GoonWorld
{
    class Image;
    class Text;
    class Panel : public IDraw, public IUpdate, public IEnable
    {
    public:
        virtual ~Panel();
        void AddImage(Image *image);
        void AddText(Text *image);
        inline void AddUIUpdateObject(IUpdate *obj) { UIUpdateObjects.push_back(obj); }
        inline void AddUIDrawObject(IDraw *obj) { UIDrawObjects.push_back(obj); }
        virtual void Visible(bool isVisible) override;
        bool IsVisible() override;
        virtual void Update() override;
        virtual void Draw() override;
        std::vector<IDraw *> UIDrawObjects;
        std::vector<IUpdate *> UIUpdateObjects;

        bool IsEnabled() const override;
        void Enabled(bool isEnabled) override;
        void OnEnabled() override;
        void OnDisabled() override;

    private:
        bool _enabled = true;
    };

}