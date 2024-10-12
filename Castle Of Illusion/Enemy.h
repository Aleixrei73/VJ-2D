#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE


#include "Sprite.h"
#include "TileMap.h"

class Enemy
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setVelocity(int vel);

	glm::ivec2 getHitBox();
	glm::ivec2 getPosition();

private:
	glm::ivec2 tileMapDispl, posEnemy, hitBox;
	TileMap *map;
	Texture spritesheet;
	Sprite *sprite;
	int velocity;
};

#endif
