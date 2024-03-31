#pragma once
namespace GoonWorld
{
    class ILoadContent
    {
    public:
        virtual std::string &GetContentName() = 0;
        virtual void Load() = 0;
        virtual void Unload() = 0;
        virtual bool IsLoaded() = 0;
        virtual unsigned int GetAssetID() const = 0;
        virtual void SetAssetID(unsigned int ID) = 0;
        virtual ~ILoadContent() {}
    };

}