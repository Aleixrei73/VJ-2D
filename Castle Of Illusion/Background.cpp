#include "Background.h"

void Background::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {

	spritesheet.loadFromFile("images/FONDO.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(100*32, 32*7*3), glm::vec2(1, 1), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(0);

	sprite->setPosition(glm::vec2(float(tileMapPos.x + 0), float(tileMapPos.y + 14*32)));
}

void Background::render() {
	sprite->render();
}

void Background::setPosition(const glm::ivec2 & pos) {
	sprite->setPosition(pos);
}
