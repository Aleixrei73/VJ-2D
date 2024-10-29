#pragma once

#include "Enemy.h"

class Projectile : public Enemy {
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	void shoot(const glm::ivec2 &pos, int dir);
	bool isShot();

private:
	bool shot;
};
