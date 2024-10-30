#include "SoundManager.h"
#include <SDL.h>
#include <SDL_mixer.h>

void SoundManager::init() {
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		std::cerr << "Error al inicializar SDL: " << SDL_GetError() << std::endl;
		return;
	}

	// Inicializar SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cerr << "Error al inicializar SDL_mixer: " << Mix_GetError() << std::endl;
		SDL_Quit();
		return;
	}

	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		std::cerr << "Error al inicializar SDL: " << SDL_GetError() << std::endl;
		return;
	}

	// Inicializar SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cerr << "Error al inicializar SDL_mixer: " << Mix_GetError() << std::endl;
		SDL_Quit();
		return;
	}

	// Cargar un archivo de sonido
	Mix_Chunk* background = Mix_LoadWAV("audio/background_music.wav");
	if (!background) {
		std::cerr << "Error al cargar el sonido: " << Mix_GetError() << std::endl;
		Mix_CloseAudio();
		SDL_Quit();
		return;
	}

	// Reproducir el sonido
	int channel = Mix_PlayChannel(-1, background, 0);

	Mix_Volume(channel, 64);
}

void SoundManager::playSound(SoundType type) {
	if (type == SoundType::JUMP) {
		Mix_Chunk* sound = Mix_LoadWAV("audio/jump.wav");
		if (!sound) {
			std::cerr << "Error al cargar el sonido: " << Mix_GetError() << std::endl;
			Mix_CloseAudio();
			SDL_Quit();
			return;
		}

		// Reproducir el sonido
		int channel = Mix_PlayChannel(-1, sound, 0);
		Mix_Volume(channel, 64);
		return;
	}

	if (type == SoundType::COIN) {
		Mix_Chunk* sound = Mix_LoadWAV("audio/coin.wav");
		if (!sound) {
			std::cerr << "Error al cargar el sonido: " << Mix_GetError() << std::endl;
			Mix_CloseAudio();
			SDL_Quit();
			return;
		}

		// Reproducir el sonido
		Mix_PlayChannel(-1, sound, 0);
		return;
	}

	if (type == SoundType::HIT) {
		Mix_Chunk* sound = Mix_LoadWAV("audio/hit.wav");
		if (!sound) {
			std::cerr << "Error al cargar el sonido: " << Mix_GetError() << std::endl;
			Mix_CloseAudio();
			SDL_Quit();
			return;
		}

		// Reproducir el sonido
		Mix_PlayChannel(-1, sound, 0);
		return;
	}

	if (type == SoundType::KILL) {
		Mix_Chunk* sound = Mix_LoadWAV("audio/kill.wav");
		if (!sound) {
			std::cerr << "Error al cargar el sonido: " << Mix_GetError() << std::endl;
			Mix_CloseAudio();
			SDL_Quit();
			return;
		}

		// Reproducir el sonido
		Mix_PlayChannel(-1, sound, 0);
		return;
	}

	if (type == SoundType::THROW) {
		Mix_Chunk* sound = Mix_LoadWAV("audio/throw.wav");
		if (!sound) {
			std::cerr << "Error al cargar el sonido: " << Mix_GetError() << std::endl;
			Mix_CloseAudio();
			SDL_Quit();
			return;
		}

		// Reproducir el sonido
		Mix_PlayChannel(-1, sound, 0);
		return;
	}

	if (type == SoundType::BLOCK_BREAK) {
		Mix_Chunk* sound = Mix_LoadWAV("audio/break.wav");
		if (!sound) {
			std::cerr << "Error al cargar el sonido: " << Mix_GetError() << std::endl;
			Mix_CloseAudio();
			SDL_Quit();
			return;
		}

		// Reproducir el sonido
		Mix_PlayChannel(-1, sound, 0);
		return;
	}
}
