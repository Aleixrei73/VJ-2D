#include "Consumable.h"

#define MAX_FALL_VELOCITY 7

enum ConsumableAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};

void Consumable::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram) {

	spritesheet.loadFromFile("images/barrilete.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(0);
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(-1, -1));
	acceleration = glm::vec2(0.1, 0.2);
	velocity = glm::vec2(0, 0);
	hitBox = glm::ivec2(16, 16);
}

void Consumable::update(int deltaTime) {

	if (!map->collisionMoveDown(position, hitBox, &position.y)) {

		velocity.y += acceleration.y* deltaTime / 10;

		if (velocity.y > MAX_FALL_VELOCITY) velocity.y = MAX_FALL_VELOCITY;

		position.y += int(velocity.y);

		if (map->collisionMoveDown(position, hitBox, &position.y)) {
			velocity.y = 0;
		}

	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));

}

void Consumable::render() {
	sprite->render();
}

void Consumable::drop(const glm::ivec2 & pos) {
	position.x = pos.x + 8;
	position.y = pos.y - 8;
	velocity.y = -7;
}

void Consumable::setType(ConsumableType itemType) {

	type = itemType;
}
