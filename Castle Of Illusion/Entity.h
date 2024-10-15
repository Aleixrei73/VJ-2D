#pragma once

#include "Sprite.h"
#include "TileMap.h"

class Entity {

public:

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::ivec2 &pos);
	void setVelocity(const glm::vec2 &vel);
	void setHorizontalVelocity(float vel);
	void setVerticalVelocity(float vel);
	void setAcceleration(const glm::vec2 &acc);

	glm::ivec2 getHitBox() const;
	glm::ivec2 getPosition() const;
	glm::vec2 getVelocity() const;
	glm::vec2 getAcceleration() const;

protected:
	glm::ivec2 tileMapDispl, position, hitBox;
	glm::vec2 velocity, acceleration;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
};
