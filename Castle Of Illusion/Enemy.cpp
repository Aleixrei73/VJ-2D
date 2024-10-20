#include "Enemy.h"

enum EnemyAnims
{
	MOVE_LEFT, MOVE_RIGHT, DIE_LEFT, DIE_RIGHT
};

void Enemy::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram) {

	spritesheet.loadFromFile("images/tree.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(24, 32), glm::vec2(1.f/6, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(MOVE_LEFT, 2);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5f + 1/6.f, 0.f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 2);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 6.f, 0.f));

	sprite->setAnimationSpeed(DIE_LEFT, 2);
	sprite->addKeyframe(DIE_LEFT, glm::vec2(0.5f + 1 / 3.f, 0.f));

	sprite->setAnimationSpeed(DIE_RIGHT, 2);
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(1 / 3.f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	acceleration = glm::vec2(0.1, 0.2);
	velocity = glm::vec2(0,0);
	hitBox = glm::ivec2(24, 32);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));

}

void Enemy::update(int deltaTime) {

	if (dying) {
		velocity.y += acceleration.y *deltaTime / 10;
		position.y += int(velocity.y);
		if (position.y - hitBox.y > MAX_DEAD_POSITION) {
			death = true;
		}
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
		return;
	}

	sprite->update(deltaTime);

	position.x += int(velocity.x);

	if (velocity.x > 0) {

		if (sprite->animation() != MOVE_RIGHT) {
			sprite->changeAnimation(MOVE_RIGHT);
		}

		if (map->collisionMoveRight(position, hitBox) || map->isEdgeRight(position, hitBox)) {
			position.x -= int(velocity.x);
			velocity.x = velocity.x * -1;
		}

	}

	else if (velocity.x < 0) {

		if (sprite->animation() != MOVE_LEFT) {
			sprite->changeAnimation(MOVE_LEFT);
		}

		if (map->collisionMoveLeft(position, hitBox) || map->isEdgeLeft(position, hitBox) ) {
			position.x -= int(velocity.x);
			velocity.x = velocity.x * -1;
		}
	}



	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));


}

void Enemy::render()
{
	sprite->render();
}

void Enemy::die() {

	if (sprite->animation() == MOVE_RIGHT) {
		sprite->changeAnimation(DIE_RIGHT);
	}
	else {
		sprite->changeAnimation(DIE_LEFT);
	}

	dying = true;
	velocity.y = -7;
}


