#include "Barrel.h"

#define MAX_FALL_VELOCITY 7

enum BarrelAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};

void Barrel::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram, bool expl) {

	spritesheet.loadFromFile("images/barrilete.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	explosive = expl;
	exploded = false;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
	acceleration = glm::vec2(0.1, 0.2);
	velocity = glm::vec2(0, 0);
	hitBox = glm::ivec2(32, 32);
	state = FREE;

}

void Barrel::update(int deltaTime) {

	if (velocity.x > 0) {
		velocity.x -= acceleration.x * deltaTime/10;
		position.x += int(velocity.x);

		if (velocity.x < 0) velocity.x = 0;

		if (map->collisionMoveRight(position, hitBox)) {
			position.x -= int(velocity.x);
			velocity.x = 0;
		}
	}

	else if (velocity.x < 0) {

		velocity.x += acceleration.x * deltaTime / 10;

		if (velocity.x > 0) velocity.x = 0;

		position.x += int(velocity.x);

		if (map->collisionMoveLeft(position, hitBox)) {
			position.x -= int(velocity.x);
			velocity.x = 0;
		}

	}

	if (!map->collisionMoveDown(position, hitBox, &position.y)) {

		velocity.y += acceleration.y* deltaTime / 10;

		if (velocity.y > MAX_FALL_VELOCITY) velocity.y = MAX_FALL_VELOCITY;

		position.y += int(velocity.y);

		if (map->collisionMoveDown(position, hitBox, &position.y)) {
			velocity.y = 0;
			if (explosive && (state == THROWED)) {
				explode();
				exploded = true;
			}
		}

	}

	if (state == THROWED && velocity.x == 0 && velocity.y == 0) state = FREE;

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));

}

void Barrel::render() {
	sprite->render();
}

void Barrel::setState(State st) {
	state = st;
}

State Barrel::getState() {
	return state;
}

void Barrel::explode() {

}
