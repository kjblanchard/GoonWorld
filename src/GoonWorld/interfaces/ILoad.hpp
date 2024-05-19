namespace GoonWorld
{
    class ILoad
    {
    public:
        virtual void Load() = 0;
        virtual void Unload() = 0;
        virtual bool IsLoaded() = 0;
        virtual ~ILoad() {}
    };
}