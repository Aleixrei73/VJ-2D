#pragma once

#include "Sprite.h"
#include "TileMap.h"

#define MAX_DEAD_POSITION 900

class Entity {

public:

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::ivec2 &pos);
	void setVelocity(const glm::vec2 &vel);
	void setHorizontalVelocity(float vel);
	void setVerticalVelocity(float vel);
	void setAcceleration(const glm::vec2 &acc);
	void setEdgePointer(int *edge);
	void setDeath(bool dead);
	virtual void die();

	glm::ivec2 getHitBox() const;
	glm::ivec2 getPosition() const;
	glm::vec2 getVelocity() const;
	glm::vec2 getAcceleration() const;
	bool isDead() const;
	bool isDying() const;

protected:
	glm::ivec2 tileMapDispl, position, hitBox;
	glm::vec2 velocity, acceleration;
	int *mapEdge;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	bool dying;
	bool death;
};
