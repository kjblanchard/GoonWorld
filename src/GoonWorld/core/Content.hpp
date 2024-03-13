#pragma once
namespace GoonWorld
{
    enum class ContentTypes
    {
        Default,
        Surface,
        Texture,
        Bgm,
        Sfx,
    };

    class Content
    {
    public:
        static void *LoadContent(ContentTypes contentType, const char *filename);
        template <typename T>
        static T *GetLoadedContentOfType(const char *filename);
        static void *GetLoadedContent(const char *filename);
        static void ClearContent();

    private:
    };

    template <typename T>
    T *Content::GetLoadedContentOfType(const char *filename)
    {
        auto content = GetLoadedContent(filename);
        return (T *)content;
    }

}
