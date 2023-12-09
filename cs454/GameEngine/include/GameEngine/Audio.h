#pragma once

#include <SDL2/SDL_mixer.h>
#include <string>

namespace GameEngine {

class Audio {
   private:
	Mix_Chunk* sound;
	bool loop;
	int channel;

   public:
	Audio() : sound(nullptr), loop(false) { channel = -1; }
	~Audio() { this->Destroy(); }

	bool Load(std::string path);
	void Destroy();

	void Play();
	void Stop();
	void Pause();
	void Resume();

	void SetVolume(int volume);
	int GetVolume() const;

	static void SetMasterVolume(int volume);
	static int GetMasterVolume();

	bool IsPlaying() const;
	bool IsLooped() const { return loop; }
	void SetLoop(bool loop) { this->loop = loop; }
};
}  // namespace GameEngine