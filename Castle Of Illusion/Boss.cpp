#include "boss.h"
#include<iostream>

enum BossAnims
{
	STAY_RIGHT, DIE, SPIN, STAY_LEFT
};

void Boss::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {

	spritesheet.loadFromFile("images/boss.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(1.f / 8, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(SPIN, 5);
	sprite->addKeyframe(SPIN, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(SPIN, glm::vec2(0.f + 1 / 8.f, 0.f));

	sprite->setAnimationSpeed(STAY_RIGHT, 5);
	sprite->addKeyframe(STAY_RIGHT, glm::vec2(2 / 8.f, 0.f));
	sprite->addKeyframe(STAY_RIGHT, glm::vec2(3 / 8.f, 0.f));

	sprite->setAnimationSpeed(STAY_LEFT, 2);
	sprite->addKeyframe(STAY_LEFT, glm::vec2(5 / 8.f, 0.f));
	sprite->addKeyframe(STAY_LEFT, glm::vec2(6 / 8.f, 0.f));

	sprite->setAnimationSpeed(DIE, 10);
	sprite->addKeyframe(DIE, glm::vec2(4 / 8.f, 0.f));
	sprite->addKeyframe(DIE, glm::vec2(7 / 8.f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	acceleration = glm::vec2(0.1, 0.2);
	velocity = glm::vec2(2, 2);
	hitBox = glm::ivec2(50, 40);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
	lives = 3;

	Projectile* proj1 = new Projectile();
	proj1->init(tileMapPos, shaderProgram);
	proj1->setPosition(glm::ivec2(-1, -1));
	proj1->setEdgePointer(mapEdge);

	Projectile* proj2 = new Projectile();
	proj2->init(tileMapPos, shaderProgram);
	proj2->setPosition(glm::ivec2(-1, -1));
	proj2->setEdgePointer(mapEdge);

	Projectile* proj3 = new Projectile();
	proj3->init(tileMapPos, shaderProgram);
	proj3->setPosition(glm::ivec2(-1, -1));
	proj3->setEdgePointer(mapEdge);


	projectiles.push_back(proj1);
	projectiles.push_back(proj2);
	projectiles.push_back(proj3);

	projectiles[0]->setAcceleration(glm::vec2(0, 0));
	projectiles[1]->setAcceleration(glm::vec2(0, 0));
	projectiles[2]->setAcceleration(glm::vec2(0, 0));
}

void Boss::update(int deltaTime) {
	projectiles[0]->update(deltaTime);
	projectiles[1]->update(deltaTime);
	projectiles[2]->update(deltaTime);
	if (dying) {
		velocity.y += acceleration.y * deltaTime / 10;
		position.y += int(velocity.y);
		if (position.y - hitBox.y > *mapEdge) {
			death = true;
		}
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
		return;
	}

	sprite->update(deltaTime);




	if (!spining) {
		currentTime += deltaTime;
		if (currentTime < 1000 && currentTime > 900) {

			if (velocity.x > 0) {

				projectiles[0]->Bshoot(position + glm::ivec2(hitBox.x / 2, -hitBox.y - 10), 5, 0);
				projectiles[1]->Bshoot(position + glm::ivec2(hitBox.x / 2, -hitBox.y + -5), 5, 1);
				projectiles[2]->Bshoot(position + glm::ivec2(hitBox.x / 2, -hitBox.y - 15), 5, -1);
			}
			else {
				projectiles[0]->Bshoot(position + glm::ivec2(hitBox.x / 2, -hitBox.y - 10), -5, 0);
				projectiles[1]->Bshoot(position + glm::ivec2(hitBox.x / 2, -hitBox.y + -5), -5, 1);
				projectiles[2]->Bshoot(position + glm::ivec2(hitBox.x / 2, -hitBox.y - 15), -5, -1);

			}

		}

		if (currentTime > 3000) {
			spining = true;
			currentTime = 0;
		}
	}
	else {
		position.x += int(velocity.x);
		if (sprite->animation() != SPIN) {
			sprite->changeAnimation(SPIN);
		}

		if (map->collisionMoveRight(position, hitBox) || map->isEdgeRight(position, hitBox) || map->collisionMoveLeft(position, hitBox) || map->isEdgeLeft(position, hitBox)) {
			position.x -= int(velocity.x);
			velocity.x = velocity.x * -1;
			spining = false;
			if (velocity.x > 0) {
				sprite->changeAnimation(STAY_RIGHT);
			}
			else {
				sprite->changeAnimation(STAY_LEFT);
			}
		}


	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));


}




void Boss::render()
{
	sprite->render();
	for (Projectile* proj : projectiles) {
		proj->render();
	}
}

void Boss::die() {

	if (!spining) {
		lives -= 1;
		sprite->changeAnimation(DIE);
	}
	if (lives <= 0) {
		sprite->changeAnimation(DIE);
		dying = true;
		velocity.y = -7;
	}

}
vector<Projectile*> Boss::getProjectiles()
{
	return projectiles;
}



