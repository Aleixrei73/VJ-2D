#include "Projectile.h"

void Projectile::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram) {
	spritesheet.loadFromFile("images/Polen.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(7, 8), glm::vec2(1.f / 2, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(0, 1);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(0, glm::vec2(0.5f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	acceleration = glm::vec2(0.1, 0.1);
	velocity = glm::vec2(0, 0);
	hitBox = glm::ivec2(7, 8);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
	shot = false;
}

void Projectile::update(int deltaTime) {

	if (shot) {
		velocity.y += acceleration.y* deltaTime / 10;

		position.y += int(velocity.y);
		position.x += int(velocity.x);

		sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));

		if (position.y > *mapEdge) {
			shot = false;
			position = glm::vec2(-1, -1);
			velocity.y = 0;
		}
	}
}

void Projectile::render() {
	if (shot) sprite->render();
}

void Projectile::shoot(const glm::ivec2 & pos, int dir) {
	shot = true;
	velocity.x = 1 * dir;
	velocity.y = -5;
	position = pos;
	sprite->setPosition(pos);
}

bool Projectile::isShot()
{
	return shot;
}
