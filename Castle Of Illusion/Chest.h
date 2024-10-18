#pragma once

#include "Entity.h"
#include "Consumable.h"

class Chest : public Entity {

public:

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderPrograms, ConsumableType itemType);
	void render();
	Consumable * open();
	bool isOpened();

private:
	Consumable *item;
	bool opened;

};
