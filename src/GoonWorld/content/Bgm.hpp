#pragma once
#include <string>
#include <GoonWorld/interfaces/ILoadContent.hpp>
typedef struct gsBgm gsBgm;
namespace GoonWorld
{
    class Bgm : public ILoadContent
    {
    public:
        // static Bgm *BgmFactory(std::string bgmName);
        static Bgm *BgmFactory(std::string bgmName, float start = 0, float end = 0);
        static inline std::string GetLoadPath(std::string &title) { return "assets/audio/" + title + ".ogg"; }

        inline bool IsLoaded() override { return _bgm != nullptr; }
        inline unsigned int GetAssetID() const override { return _id; };
        inline void SetAssetID(unsigned int ID) override { _id = ID; };
        Bgm(std::string &name, float start = 0, float end = 0);
        void Play(int loops = -1, float volume = 1.0);
        ~Bgm();
        inline std::string &GetContentName() override { return _bgmName; }
        void Load() override;
        void Unload() override;

    private:
        std::string _bgmName;
        float _bgmStart, _bgmEnd;
        gsBgm *_bgm = nullptr;
        unsigned int _id;
    };

}