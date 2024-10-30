#include "Barrel.h"

#define MAX_FALL_VELOCITY 7

enum BarrelAnims
{
	STAND, EXPLODE
};

Barrel::~Barrel() {
	delete sprite;
}

void Barrel::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram, bool expl) {

	spritesheet.loadFromFile("images/objects.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1/11.f, 1/3.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	float sprite_gap = 1 / 11.f;

	sprite->setAnimationSpeed(STAND, 8);
	if (!expl) sprite->addKeyframe(STAND, glm::vec2(0.f, 1/3.f));
	else sprite->addKeyframe(STAND, glm::vec2(2*sprite_gap, 0.f));

	sprite->setAnimationSpeed(EXPLODE, 11);
	for (int i = 0; i < 11; i++) {
		sprite->addKeyframe(EXPLODE, glm::vec2(i*sprite_gap, 2/3.f));
	}

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	explosive = expl;
	exploded = false;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
	acceleration = glm::vec2(0, 0.2);
	velocity = glm::vec2(0, 0);
	hitBox = glm::ivec2(16, 16);
	state = FREE;

}

void Barrel::update(int deltaTime) {

	if (exploded) {
		sprite->update(deltaTime);
		explodeTime += deltaTime;
		if (explodeTime > 1000) {
			death = true;
		}
		return;
	}

	if (state == PICKED || state == FREE) return;

	if (velocity.x > 0) {
		velocity.x -= acceleration.x * deltaTime/10;
		position.x += int(velocity.x);

		if (velocity.x < 0) velocity.x = 0;

		if (map->collisionMoveRight(position, hitBox)) {
			position.x -= int(velocity.x);
			velocity.x = 0;
			if (explosive) {
				explode();
			}
		}
	}

	else if (velocity.x < 0) {

		velocity.x += acceleration.x * deltaTime / 10;

		if (velocity.x > 0) velocity.x = 0;

		position.x += int(velocity.x);

		if (map->collisionMoveLeft(position, hitBox)) {
			position.x -= int(velocity.x);
			velocity.x = 0;
			if (explosive) {
				explode();
			}
		}

	}

	if (!map->collisionMoveDown(position, hitBox, &position.y)) {

		velocity.y += acceleration.y* deltaTime / 10;

		if (velocity.y > MAX_FALL_VELOCITY) velocity.y = MAX_FALL_VELOCITY;

		position.y += int(velocity.y);

		if (map->collisionMoveDown(position, hitBox, &position.y)) {
			velocity.y = 0;
			velocity.x = 0;
			if (explosive) {
				explode();
			}
		}

		if (position.y > *mapEdge) death = true;

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

bool Barrel::isExplosive() const
{
	return explosive;
}

void Barrel::explode() {
	sprite->changeAnimation(EXPLODE);
	explodeTime = 0;
	exploded = true;
}

bool Barrel::hasExploded() const
{
	return exploded;
}
