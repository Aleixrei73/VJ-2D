#pragma once

#include "Entity.h"

class Chest : public Entity {

public:

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderPrograms);
	void render();
	void open();
	bool isOpened();

private:

	bool opened;

};
