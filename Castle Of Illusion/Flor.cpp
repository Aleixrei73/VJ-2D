#include "Flor.h"

enum FlowerAnims {STAND, PREPARED, SHOOT, DIE};

Flor::~Flor() {
	delete projectiles[0];
	delete projectiles[1];
}

void Flor::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram, Player *pl) {
	spritesheet.loadFromFile("images/Flor.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(17, 25), glm::vec2(1.f / 4, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(STAND, 1);
	sprite->addKeyframe(STAND, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(PREPARED, 1);
	sprite->addKeyframe(PREPARED, glm::vec2(0.25f, 0.f));

	sprite->setAnimationSpeed(SHOOT, 1);
	sprite->addKeyframe(SHOOT, glm::vec2(0.5f, 0.f));

	sprite->setAnimationSpeed(DIE, 1);
	sprite->addKeyframe(DIE, glm::vec2(0.75f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	acceleration = glm::vec2(0.1, 0.2);
	velocity = glm::vec2(0, 0);
	hitBox = glm::ivec2(17, 25);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
	player = pl;
	shooting = false;
	currentTime = 0;

	Projectile* proj1 = new Projectile();
	proj1->init(tileMapPos, shaderProgram);
	proj1->setPosition(glm::ivec2(-1, -1));
	proj1->setEdgePointer(mapEdge);
	
	Projectile* proj2 = new Projectile();
	proj2->init(tileMapPos, shaderProgram);
	proj2->setPosition(glm::ivec2(-1, -1));
	proj2->setEdgePointer(mapEdge);

	projectiles.push_back(proj1);
	projectiles.push_back(proj2);
}

void Flor::update(int deltaTime) {

	sprite->update(deltaTime);
	projectiles[0]->update(deltaTime);
	projectiles[1]->update(deltaTime);

	if (dying) {
		velocity.y += acceleration.y *deltaTime / 10;
		position.y += int(velocity.y);
		if (position.y - hitBox.y > *mapEdge) {
			if (!projectiles[0]->isShot() && !projectiles[1]->isShot()) {
				death = true;
				shooting = false;
				for (int i = 0; i < 2; i++) {
					delete (projectiles[i]);
				}
			}
		}
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
		return;
	}

	int player_x = player->getPosition().x;
	int flower_x = position.x;
	int height = position.y / (7 * map->getTileSize());
	int playerHeight = player->getPosition().y / (7 * map->getTileSize());

	if (abs(player_x - flower_x) < 5 * map->getTileSize() && height == playerHeight) {
		
		currentTime += deltaTime;

		if (currentTime > 1500) {
			sprite->changeAnimation(SHOOT);
			shooting = true;
			projectiles[0]->shoot(position + glm::ivec2(hitBox.x / 2, -hitBox.y), -1);
			projectiles[1]->shoot(position + glm::ivec2(hitBox.x / 2, -hitBox.y), 1);
			currentTime = 0;
		}

		if (currentTime > 500) {
			if (sprite->animation() != PREPARED) sprite->changeAnimation(PREPARED);
		}
		

	}
	else {
		if (sprite->animation() != STAND) sprite->changeAnimation(STAND);
		currentTime = 0;
		shooting = false;
	}

}

void Flor::render() {
	sprite->render();
	for (Projectile* proj : projectiles) {
		proj->render();
	}
}

void Flor::die() {
	
	sprite->changeAnimation(DIE);
	dying = true;
	velocity.y = -7;
}

bool Flor::isFlower()
{
	return true;
}

bool Flor::isShooting()
{
	return shooting;
}

vector<Projectile*> Flor::getProjectiles()
{
	return projectiles;
}
