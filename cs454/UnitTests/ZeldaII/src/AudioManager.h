#pragma once
#include <GameEngine/Audio.h>
#include <GameEngine/Resources.h>
#include <string>
#include <unordered_map>

class AudioManager final {
   private:
	AudioManager() {}
	AudioManager(const AudioManager&) = delete;
	AudioManager& operator=(const AudioManager&) = delete;

	static AudioManager manager;
	std::unordered_map<std::string, GameEngine::Audio*> audios;

	int sfxVol = 0;
	public:
	static auto Get() -> AudioManager& { return manager; }

	void SetSoundEffectVolume(int vol) { sfxVol = vol; }

	GameEngine::Audio* GetAudio(const std::string& name) {
		auto it = audios.find(name);
		if (it == audios.end()) {
			GameEngine::Audio* audio = new GameEngine::Audio();
			audio->Load(GameEngine::Resources::Get().GetAsset(name));
			audios[name] = audio;
		}
		return audios[name];
	}
	
	void Stop(const std::string& name) { GetAudio(name)->Stop(); }

	void Play(const std::string& name, int vol) {
		GameEngine::Audio* audio = GetAudio(name);
		audio->Play();
		audio->SetVolume(vol);
	}

	void PlayEffect(const std::string& name) {
		Play(name, sfxVol);
	}

	void CleanUp() {
		for (auto& audio : audios) {
			audio.second->Stop();
			delete audio.second;
		}
		audios.clear();
	}
};