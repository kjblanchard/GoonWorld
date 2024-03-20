#pragma once
namespace GoonWorld
{
    class ILoadContent;
    enum class ContentTypes
    {
        Default,
        Surface,
        Texture,
        Sfx,
        Text,
    };

    class Content
    {
    public:
        static void *LoadContent(ContentTypes contentType, const char *filename);
        template <typename T>
        static T *GetLoadedContentOfType(const char *filename);
        static void *GetLoadedContent(const char *filename);

        static void AddContent(ILoadContent *content);
        static void LoadAllContent();
        static ILoadContent *GetContent(std::string &name);
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
