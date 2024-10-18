#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 16
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 0
#define INIT_PLAYER_Y_TILES 33


Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}


void Scene::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	Enemy *enemy = new Enemy();
	enemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	enemy->setPosition(glm::vec2(45 * map->getTileSize(), 32 * map->getTileSize()));
	enemy->setTileMap(map);
	enemy->setHorizontalVelocity(1);
	enemies.push_back(enemy);

	Barrel *barrel = new Barrel();
	barrel->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, false);
	barrel->setPosition(glm::vec2(30 * map->getTileSize(), 32 * map->getTileSize()));
	barrel->setTileMap(map);
	barrels.push_back(barrel);
	Chest *chest = new Chest();
	chest->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, POINTS);
	chest->setPosition(glm::vec2(25 * map->getTileSize(), 35 * map->getTileSize()));
	chest->setTileMap(map);
	chests.push_back(chest);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	currentTime = 0.0f;
}

Direction Scene::isCollision(Entity *player, Entity *enemy) {
	glm::ivec2 posPlayer = player->getPosition();
	glm::ivec2 hitBoxPlayer = player->getHitBox();
	glm::ivec2 posEnemy = enemy->getPosition();
	glm::ivec2 hitBoxEnemy = enemy->getHitBox();

	int playerTopHitBox = posPlayer.y - hitBoxPlayer.y;
	int enemyTopHitBox = posEnemy.y - hitBoxEnemy.y;
	int playerRightBorder = posPlayer.x + hitBoxPlayer.x;
	int enemyRightBorder = posEnemy.x + hitBoxEnemy.x;

	if (posPlayer.y >= enemyTopHitBox && playerTopHitBox <= posEnemy.y) {

		if (player->getVelocity().y >= 0 && posPlayer.y <= (enemyTopHitBox + hitBoxEnemy.y*0.20)) {
			if ((playerRightBorder >= posEnemy.x && playerRightBorder <= enemyRightBorder) ||
				(posPlayer.x <= enemyRightBorder && posPlayer.x >= posEnemy.x)) return UP;
		}

		if (playerRightBorder >= posEnemy.x && playerRightBorder <= enemyRightBorder) return LEFT;

		if (posPlayer.x <= enemyRightBorder && posPlayer.x >= posEnemy.x) return RIGHT;

		return NONE;
	}

	return NONE;
}

void Scene::updateInteractions(Player *player, Enemy *enemy) {
	Direction dir = isCollision(player, enemy);

	if (dir == NONE) return;

	if (player->getAction() != PlayerAction::ATTACKING || dir != UP) {

		if (dir == LEFT) {
			player->setHorizontalVelocity(-5);
			glm::ivec2 hitPosition = glm::ivec2(enemy->getPosition().x, player->getPosition().y) - glm::ivec2(player->getHitBox().x, 0);
			player->setPosition(hitPosition);
			return;
		}

		if (dir == RIGHT) {
			player->setHorizontalVelocity(5);
			glm::ivec2 hitPosition = glm::ivec2(enemy->getPosition().x, player->getPosition().y) + glm::ivec2(enemy->getHitBox().x,0);
			player->setPosition(hitPosition);
			return;
		}

		if (dir == UP) {
			glm::ivec2 hitPosition = glm::ivec2(player->getPosition().x, enemy->getPosition().y) - glm::ivec2(0, enemy->getHitBox().y);
			player->setPosition(hitPosition);
			player->setJump(-5);
			return;
		}
	}

	glm::ivec2 hitPosition = glm::ivec2(player->getPosition().x, enemy->getPosition().y) - glm::ivec2(0, enemy->getHitBox().y);
	player->setPosition(hitPosition);
	player->setJump(-5);
	enemy->die();
}

void Scene::updateInteractions(Player * player, Barrel * barrel) {

	Direction dir = isCollision(player, barrel);

	if (!interacting && Game::instance().getKey(GLFW_KEY_V)) {
		if (barrel->getState() == PICKED) {
			interacting = true;
			barrel->setState(THROWED);
			glm::vec2 newVelocity = glm::vec2(player->getVelocity().x * 3, player->getVelocity().y - 7);
			if (player->getVelocity().x == 0) newVelocity.y = 0;
			barrel->setVelocity(newVelocity);
			player->setPicking(false);
		}
		else if (!player->isPicking() && dir != NONE) {
			interacting = true;
			barrel->setState(PICKED);
			barrel->setVelocity(glm::vec2(0, 0));
			player->setPicking(true);
		}

	}

	else if (barrel->getState() == PICKED) {
		glm::ivec2 newPosition = glm::ivec2(player->getPosition().x, player->getPosition().y - player->getHitBox().y);
		barrel->setPosition(newPosition);
	}

	else if (dir == UP) {
		glm::ivec2 newPosition = glm::ivec2(player->getPosition().x, barrel->getPosition().y - barrel->getHitBox().y);
		player->setPosition(newPosition);
		player->setAction(PlayerAction::GROUNDED);
		//We must check if player needs to jump since the player update will make it think it is in the air
		if (Game::instance().getKey(GLFW_KEY_SPACE)) {
			player->setAction(PlayerAction::JUMPING);
			player->setVerticalVelocity(-7.0);
		}
	}
}

void Scene::updateInteractions(Player * player, Chest * chest) {

	Direction dir = isCollision(player, chest);

	if (player->getAction() == PlayerAction::ATTACKING && dir == UP) {
		glm::ivec2 hitPosition = glm::ivec2(player->getPosition().x, chest->getPosition().y) - glm::ivec2(0, chest->getHitBox().y);
		player->setPosition(hitPosition);
		player->setJump(-5);
		Consumable *item = chest->open();
		item->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		item->setTileMap(map);
		item->drop(chest->getPosition());
		items.push_back(item);
		chest->die();
	}
}

void Scene::checkKillCollision(Entity * killer, Entity * target) {
	Direction dir = isCollision(killer, target);
	if (dir != NONE) target->die();
}

void Scene::update(int deltaTime) {

	currentTime += deltaTime;
	if (interacting) interacting = Game::instance().getKey(GLFW_KEY_V);

	//Updates all entites and its collides with the player

	player->update(deltaTime);

	for (Enemy* enemy : enemies) {
		enemy->update(deltaTime);
		if (!enemy->isDying()) updateInteractions(player, enemy);
	}

	for (Barrel* barrel : barrels) {
		barrel->update(deltaTime);
		updateInteractions(player, barrel);
	}

	for (Consumable* item : items) {
		item->update(deltaTime);
	}

	for (Chest* chest : chests) {
		updateInteractions(player, chest);
		if (chest->isDying())chest->update(deltaTime);
	}

	//Check interaction between entities (basically checks if a barrel is hitting an enemy when throwed)

	for (Enemy* enemy : enemies) {
		for (Barrel* barrel : barrels) {
			if (barrel->getState() == THROWED) checkKillCollision(barrel, enemy);
		}
	}

	//Check if entities are dead to erase them so we do not update or render them anymore

	int n = enemies.size();
	for (int i = 0; i < n; i++) {
		if (enemies[i]->isDead()) enemies.erase(enemies.begin()+i);
	}

	n = chests.size();
	for (int i = 0; i < n; i++) {
		if (chests[i]->isDead()) chests.erase(chests.begin() + i);
	}

	n = barrels.size();
	for (int i = 0; i < n; i++) {
		if (barrels[i]->isDead()) barrels.erase(barrels.begin() + i);
	}


}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();

	for (Enemy* enemy : enemies) {
		enemy->render();
	}

	for (Barrel* barrel : barrels) {
		barrel->render();
	}

	for (Chest* chest : chests) {
		chest->render();
	}
	for (Consumable* item : items) {
		item->render();
	}

	player->render();
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



