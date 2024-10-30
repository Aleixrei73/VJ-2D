#pragma once

#include "Enemy.h"

class Projectile : public Enemy {
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	void shoot(const glm::ivec2 &pos, int dir);
	void Bshoot(const glm::ivec2& pos, int dir, int alpha);
	bool isShot();

private:
	bool shot;
};
