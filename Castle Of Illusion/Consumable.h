#pragma once

#include "Entity.h"

enum ConsumableType {POINTS, LIFE};

class Consumable : public Entity {

public:

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	void drop(const glm::ivec2 &pos);
	void setType(ConsumableType itemType);

private:
	ConsumableType type;

};