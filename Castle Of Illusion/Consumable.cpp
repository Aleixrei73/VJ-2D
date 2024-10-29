#include "Consumable.h"

#define MAX_FALL_VELOCITY 7

void Consumable::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram) {

	if (type == ConsumableType::POINTS) {
		spritesheet.loadFromFile("images/coin.png", TEXTURE_PIXEL_FORMAT_RGBA);
		sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1 / 6.f, 1), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(1);

		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(0, glm::vec2(1.f / 6.f, 0.f));
		sprite->addKeyframe(0, glm::vec2(2.f / 6.f, 0.f));
		sprite->addKeyframe(0, glm::vec2(3.f / 6.f, 0.f));
		sprite->addKeyframe(0, glm::vec2(4.f / 6.f, 0.f));
		sprite->addKeyframe(0, glm::vec2(5.f / 6.f, 0.f));

		sprite->changeAnimation(0);
	}
	else {
		spritesheet.loadFromFile("images/cake.png", TEXTURE_PIXEL_FORMAT_RGBA);
		sprite = Sprite::createSprite(glm::ivec2(16, 14), glm::vec2(1, 1), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(1);
		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
		sprite->changeAnimation(0);
	}

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(-1, -1));
	acceleration = glm::vec2(0.1, 0.2);
	velocity = glm::vec2(0, 0);
	hitBox = glm::ivec2(16, 16);
	droped = false;
}

void Consumable::update(int deltaTime) {

	sprite->update(deltaTime);

	if (droped && !map->collisionMoveDown(position, hitBox, &position.y)) {

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
	velocity.y = -10;
	droped = true;
}

void Consumable::setType(ConsumableType itemType) {

	type = itemType;
}

void Consumable::die() {
	death = true;
}

ConsumableType Consumable::getType() {
	return type;
}
