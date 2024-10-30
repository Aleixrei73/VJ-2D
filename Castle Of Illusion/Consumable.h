#pragma once

#include "Entity.h"

enum class ConsumableType {POINTS, LIFE, GEM};

class Consumable : public Entity {

public:

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	void drop(const glm::ivec2 &pos);
	void setType(ConsumableType itemType);
	void die() override;
	ConsumableType getType();

private:
	ConsumableType type;
	bool droped = false;

};