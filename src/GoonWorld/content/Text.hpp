#pragma once
#include <string>
#include <GoonWorld/interfaces/ILoadContent.hpp>
namespace GoonWorld
{
    class Text : public ILoadContent
    {
    public:
        Text(std::string stringToLoad);
        ~Text();
        void Load() override;
        void Unload(bool isVisible) override;

    private:
        std::string _stringToDisplay;
    };

}
