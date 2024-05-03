#pragma once
#include <vector>
#include <GoonWorld/base/UiObject.hpp>

namespace GoonWorld
{
    class Image;
    class Text;
    class Panel : public UiObject
    {
    public:
        virtual ~Panel();
        void AddImage(Image *image);
        void AddText(Text *image);
        inline void AddUIDrawObject(UiObject *obj) { UiObjects.push_back(std::unique_ptr<UiObject>(obj)); }
        virtual void Visible(bool isVisible) override;
        bool IsVisible() override;
        virtual void Update() override;
        virtual void Draw() override;
        std::vector<std::unique_ptr<UiObject>> UiObjects;
        bool IsEnabled() const override;
        void Enabled(bool isEnabled) override;
        void OnEnabled() override;
        void OnDisabled() override;

    private:
        bool _enabled = true;
    };

}