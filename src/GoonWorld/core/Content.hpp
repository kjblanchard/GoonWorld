#pragma once
namespace GoonEngine
{
    enum class ContentTypes
    {
        Default,
        Surface,
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