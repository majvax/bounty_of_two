#pragma once
#include <raylib-cpp.hpp>
#include <memory>
#include <string>

class SoundManager {
    static SoundManager* instance;
    std::vector<raylib::Music> music_streams;
    SoundManager() { InitAudioDevice(); }
    ~SoundManager() { CloseAudioDevice(); }
public:
    SoundManager(SoundManager &other) = delete;
    void operator=(const SoundManager &) = delete;

    static SoundManager* GetInstance() {
        if (!instance) {
            instance = new SoundManager();
        }
        return instance;
    }

    static void SetMasterVolume(float volume) { ::SetMasterVolume(volume); }
    void SetMusicVolume(int index, float volume) {
        if (index < 0 || index >= music_streams.size()) return;
        music_streams[index].SetVolume(volume);
    }
    void play(std::string filename) { 
        music_streams.emplace_back(filename); 
        music_streams.back().SetVolume(1.0f);
        music_streams.back().Play();
    }
    void update() {
        for (auto& music : music_streams) {
            music.Update();
        }
    }
};




SoundManager* SoundManager::instance = nullptr;;
