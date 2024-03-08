#pragma once
namespace GoonWorld
{
    enum class ContentTypes
    {
        Default,
        Surface,
        Texture,
        Sfx,
    };

    class Content
    {
    public:
        static void *LoadContent(ContentTypes contentType, const char *filename);
        template <typename T>
        static T *GetLoadedContentOfType(const char *filename);
        static void *GetLoadedContent(const char *filename);

    private:
    };

}