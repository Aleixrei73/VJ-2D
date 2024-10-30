#pragma once

#include <iostream>

enum class SoundType {JUMP, COIN, HIT, KILL, THROW, BLOCK_BREAK};

class SoundManager {
private:
	SoundManager() {}
public:
	
	static SoundManager& instance()
	{
		static SoundManager SM;

		return SM;
	}

	void init();

	void playSound(SoundType type);

private:

};
