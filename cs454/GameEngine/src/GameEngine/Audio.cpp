#include <GameEngine/Audio.h>
#include <iostream>
#include <assert.h>

void GameEngine::Audio::Stop() {
	if (channel >= 0) {
		Mix_HaltChannel(this->channel);
		isPaused = false;
		isPlaying = false;
	}
}

void GameEngine::Audio::Pause() {
	if (channel >= 0) {
		Mix_Pause(this->channel);
		isPaused = true;
	}
}

void GameEngine::Audio::Resume() {
	if (channel >= 0) {
		Mix_Resume(this->channel);
		isPaused = false;
	}
}

void GameEngine::Audio::SetVolume(int volume) {
	if (channel >= 0)
		Mix_Volume(channel, volume);
}

void GameEngine::Audio::SetMasterVolume(int volume) {
	Mix_MasterVolume(volume);
}

int GameEngine::Audio::GetMasterVolume() {
	return Mix_MasterVolume(-1);
}

int GameEngine::Audio::GetVolume() const {
	if (channel != -1)
		return Mix_Volume(channel, -1);
	return 0;
}

bool GameEngine::Audio::IsPlaying() const {
	return channel != -1 && isPlaying && Mix_Playing(channel) != 0;
}

bool GameEngine::Audio::IsPaused() const {
	return isPaused;
}

bool GameEngine::Audio::Load(std::string path) {
	sound = Mix_LoadWAV(path.c_str());
	if (!sound) {
		std::cerr << "SDL Mixer sound loading error: " << Mix_GetError()
				  << std::endl;
		assert(sound);
		return false;
	}
	return true;
}

void GameEngine::Audio::Destroy() {
	if (sound) {
		Mix_FreeChunk(sound);
		sound = nullptr;
	}
}

void GameEngine::Audio::Play() {
	if (sound) {
		channel = Mix_PlayChannel(-1, sound, (this->loop) ? -1 : 0);
		isPaused = false;
		isPlaying = true;
	}
}

