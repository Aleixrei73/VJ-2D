#include "Chest.h"

enum ChestAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};

void Chest::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram, ConsumableType itemType) {
	spritesheet.loadFromFile("images/chest.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(0);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	acceleration = glm::vec2(0.1, 0.2);
	velocity = glm::vec2(0, 0);
	hitBox = glm::ivec2(32, 32);
	opened = false;
	item = new Consumable();
	item->setType(itemType);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

void Chest::render() {
	sprite->render();
}

void Chest::die() {
	dying = true;
	velocity.y = -7;
}

void Chest::update(int deltaTime) {

	if (dying) {
		velocity.y += acceleration.y *deltaTime / 10;
		position.y += int(velocity.y);
		if (position.y - hitBox.y > MAX_DEAD_POSITION) {
			death = true;
		}
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
		return;
	}

}

Consumable * Chest::open() {
	opened = true;
	return item;
}

bool Chest::isOpened()
{
	return opened;
}
