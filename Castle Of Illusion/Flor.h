#pragma once

#include "Enemy.h"
#include "Player.h"
#include "Projectile.h"

class Flor : public Enemy {
public:

	~Flor();

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Player *pl);
	void update(int deltaTime) override;
	void render() override;
	void die() override;
	bool isFlower() override;
	bool isShooting();
	vector<Projectile*> getProjectiles();

private:

	Player *player;
	vector<Projectile*> projectiles;
	bool shooting;
	int currentTime;
};
