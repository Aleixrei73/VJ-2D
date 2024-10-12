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
	enemy = new Enemy();
	enemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	enemy->setPosition(glm::vec2(45 * map->getTileSize(), 29 * map->getTileSize()));
	enemy->setTileMap(map);
	enemy->setVelocity(1);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	currentTime = 0.0f;
}

Direction Scene::checkCollision(Player *player, Enemy *enemy) {
	glm::ivec2 posPlayer = player->getPosition();
	glm::ivec2 hitBoxPlayer = player->getHitBox();
	glm::ivec2 posEnemy = enemy->getPosition();
	glm::ivec2 hitBoxEnemy = enemy->getHitBox();

	int playerBottomHitBox = posPlayer.y + hitBoxPlayer.y;
	int enemyBottomHitBox = posEnemy.y + hitBoxEnemy.y;
	int playerRightBorder = posPlayer.x + hitBoxPlayer.x;
	int enemyRightBorder = posEnemy.x + hitBoxEnemy.x;

	if (playerBottomHitBox > posEnemy.y && posPlayer.y < enemyBottomHitBox) {

		if (player->getAction() == PlayerAction::FALLING) {
			if ((playerRightBorder > posEnemy.x && playerRightBorder < enemyRightBorder) ||
				(posPlayer.x < enemyRightBorder && posPlayer.x > posEnemy.x)) return UP;
		}

		if (playerRightBorder > posEnemy.x && playerRightBorder < enemyRightBorder) return LEFT;

		if (posPlayer.x < enemyRightBorder && posPlayer.x > posEnemy.x) return RIGHT;

		return NONE;
	}

	return NONE;
}

void Scene::updateInteractions(Player *player, Enemy *enemy) {
	if (player->getAction() != PlayerAction::ATTACKING) {
		Direction dir = checkCollision(player, enemy);

		if (dir == NONE) return;

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
			player->startJump(0);
			return;
		}

	}
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	enemy->update(deltaTime);
	updateInteractions(player, enemy);
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
	player->render();
	enemy->render();
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



