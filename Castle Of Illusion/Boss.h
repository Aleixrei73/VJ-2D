#pragma once
#ifndef _BOSS_INCLUDE
#define _BOSS_INCLUDE

#include "Entity.h"
#include "Enemy.h"
#include "Projectile.h"

class Boss : public Enemy
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) override;
	void update(int deltaTime) override;
	void render() override;
	void die() override;
	vector<Projectile*> getProjectiles();

private:
	int lives;
	int currentTime;
	bool spining;
	vector<Projectile*> projectiles;
};

#endif

