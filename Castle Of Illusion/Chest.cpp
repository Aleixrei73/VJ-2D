#include "Chest.h"

enum ChestAnims
{
	STAND, EXPLODE
};

Chest::~Chest() {
	delete item;
}

void Chest::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram, ConsumableType itemType) {
	spritesheet.loadFromFile("images/objects.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1/11.f, 1/3.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(STAND, 8);
	sprite->addKeyframe(STAND, glm::vec2(0.f, 0.f));

	float sprite_gap = 1 / 11.f;

	sprite->setAnimationSpeed(EXPLODE, 11);
	for (int i = 0; i < 11; i++) {
		sprite->addKeyframe(EXPLODE, glm::vec2(i*sprite_gap, 2 / 3.f));
	}

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
	sprite->changeAnimation(EXPLODE);
	velocity.y = -7;
	openTime = 0;
}

void Chest::update(int deltaTime) {

	if (dying) {
		sprite->update(deltaTime);
		openTime += deltaTime;
		if (openTime > 1000) {
			death = true;
		}
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
