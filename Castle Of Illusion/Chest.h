#pragma once

#include "Entity.h"
#include "Consumable.h"

class Chest : public Entity {

public:

	~Chest();

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderPrograms, ConsumableType itemType);
	void render();
	void die() override;
	void update(int deltaTime);
	Consumable * open();
	bool isOpened();

private:
	Consumable *item;
	bool opened;
	int openTime;

};
