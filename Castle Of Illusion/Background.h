#pragma once

#include "Sprite.h"

class Background {

public:

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderPrograms);
	void render();

private:
	Texture spritesheet;
	Sprite *sprite;

};