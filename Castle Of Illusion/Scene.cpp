#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include <algorithm>


#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 1
#define INIT_PLAYER_Y_TILES 6

#define AMPLITUDE 10


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

	interacting = false;
	god = false;
	interactingGod = false;
	height = 1;
	hit = false;

	initShaders();
	map = TileMap::createTileMap("levels/"+level+".txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	playableEdge = 7 * map->getTileSize();

	background = new Background();
	background->init(glm::ivec2(0, float(7 * map->getTileSize())), texProgram);

	gui = new GUI();
	gui->init(glm::ivec2(SCREEN_X, 10 * map->getTileSize()), texProgram, AMPLITUDE*2);
	
	initEntities();

	projection = glm::ortho(0.f, float(SCREEN_WIDTH) - 16 * map->getTileSize(), float(playableEdge + 3 * map->getTileSize()),0.f);

	currentTime = 0.0f;
}

void Scene::setLevel(string lvl) {
	level = lvl;
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

		if (player->getVelocity().y >= 0 && posPlayer.y <= (enemyTopHitBox + 10)) {
			if ((posPlayer.x <= enemyRightBorder && enemyRightBorder <= playerRightBorder) ||
				((posPlayer.x <= posEnemy.x && posEnemy.x <= playerRightBorder))) return UP;
		}

		if (playerRightBorder >= posEnemy.x && playerRightBorder <= enemyRightBorder) return LEFT;

		if (posPlayer.x <= enemyRightBorder && posPlayer.x >= posEnemy.x) return RIGHT;

		return NONE;
	}

	return NONE;
}

void Scene::updateScreen(int deltaTime) {

	float cameraShift = 0;
	float cameraLeft = float(player->getPosition().x) - AMPLITUDE * map->getTileSize() + cameraShift + 16;
	float cameraRight = float(player->getPosition().x) + AMPLITUDE * map->getTileSize() + cameraShift + 16;
	float top = playableEdge - 7 * map->getTileSize();

	if (cameraLeft < 0) {
		cameraLeft = 0.f;
		cameraRight = 2.f * AMPLITUDE * map->getTileSize();
	}
	else if (cameraRight > map->getMapWidth() * TILE_SIZE) {
		cameraRight = map->getMapWidth() * TILE_SIZE;
		cameraLeft = cameraRight - 2*AMPLITUDE * map->getTileSize();
	}

	if (player->getAction() == PlayerAction::CLIMBING) {
		if (player->getPosition().y > playableEdge) height += 1;
		else if (player->getPosition().y < top) height -= 1;
	}
	
	int objectiveEdge = height * (7 * map->getTileSize());

	if (playableEdge < objectiveEdge) {
		playableEdge += 8;
	}
	else if (playableEdge > objectiveEdge) {
		playableEdge -= 8;
	}

	top = playableEdge - 7 * map->getTileSize();

	projection = glm::ortho(cameraLeft, cameraRight, float(playableEdge + 3*map->getTileSize()), top);

	gui->update(glm::vec2(cameraLeft, float(playableEdge + 3 * map->getTileSize())), deltaTime);

}

void Scene::checkPlayerState() {

	if (gui->getLives() == 0) {
		restart();
		return;
	}

	if (gui->getTimeLeft() <= 0) {
		restart();
		return;
	}

	if (player->isDead()) {
		restart(); 
		return;
	}

}

void Scene::checkShoots(Player * player, Flor * flower) {
	vector<Projectile*> projectiles = flower->getProjectiles();

	for (Projectile* proj : projectiles) {
		updateInteractions(player, proj, true);
	}
}

void Scene::checkShoots(Player* player, Boss* boss) {
	vector<Projectile*> projectiles = boss->getProjectiles();

	for (Projectile* proj : projectiles) {
		if (!hit) updateInteractions(player, proj, true);
	}
}

void Scene::restart() {

	deleteEntities();

	initEntities();

	playableEdge = 7 * map->getTileSize();
	interacting = false;
	god = false;
	interactingGod = false;
	height = 1;
	hit = false;

	gui->setTimeLeft(200);
	gui->setLives(3);
	gui->setScore(0);

	player->setDeath(false);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setVelocity(glm::vec2(0, 0));
	if (gui->getTries() == 0) {
		gui->setTries(3);
		Game::instance().setScene(5);
		return;
	}
	gui->setTries(gui->getTries() - 1);


}

void Scene::deleteEntities() {

	delete player;

	for (int i = enemies.size() - 1; i > -1; i--) {
		delete enemies[i];
		enemies.pop_back();
	}

	for (int i = barrels.size() - 1; i > -1; i--) {
		delete barrels[i];
		barrels.pop_back();
	}

	for (int i = flowers.size() - 1; i > -1; i--) {
		delete flowers[i];
		flowers.pop_back();
	}

	for (int i = chests.size() - 1; i > -1; i--) {
		delete chests[i];
		chests.pop_back();
	}

	for (int i = items.size() - 1; i > -1; i--) {
		delete items[i];
		items.pop_back();
	}
}

void Scene::initEntities() {
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	player->setEdgePointer(&playableEdge);

	fstream fin;

	// Open an existing file
	fin.open("enemies/"+level+".csv", ios::in);

	string line, word, temp;

	

	while (true) {

		getline(fin, line);
		stringstream s(line);
		getline(s, word, ',');

		if (word == "Tree") {
			getline(s, word, ',');
			int posX = stoi(word);
			getline(s, word, ',');
			int posY = stoi(word);
			createEnemy(posX, posY);
		}
		else if (word == "Flower") {
			getline(s, word, ',');
			int posX = stoi(word);
			getline(s, word, ',');
			int posY = stoi(word);
			createFlower(posX, posY);
		}
		else if (word == "Barrel") {
			getline(s, word, ',');
			int posX = stoi(word);
			getline(s, word, ',');
			int posY = stoi(word);
			getline(s, word, ',');
			bool breakable = stoi(word) == 1;
			createBarrel(posX, posY, breakable);
		}
		else if (word == "Chest") {
			getline(s, word, ',');
			int posX = stoi(word);
			getline(s, word, ',');
			int posY = stoi(word);
			getline(s, word, ',');
			ConsumableType type;
			if (word == "Life") type = ConsumableType::LIFE;
			else type = ConsumableType::POINTS;
			createChest(posX, posY, type);
		}
		else if (word == "Item") {
			getline(s, word, ',');
			int posX = stoi(word);
			getline(s, word, ',');
			int posY = stoi(word);
			getline(s, word, ',');
			ConsumableType type;
			if (word == "Life") type = ConsumableType::LIFE;
			else type = ConsumableType::POINTS;
			createItem(posX, posY, type);
		}
		else if (word == "Boss") {
			getline(s, word, ',');
			int posX = stoi(word);
			getline(s, word, ',');
			int posY = stoi(word);
			createBoss(posX, posY);
		}
		else {
			fin.close();
			return;
		}
	}
}

void Scene::createEnemy(int posX, int posY) {
	Enemy *enemy = new Enemy();
	enemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	enemy->setPosition(glm::vec2(posX * map->getTileSize(), posY * map->getTileSize()));
	enemy->setTileMap(map);
	enemy->setHorizontalVelocity(-1);
	enemy->setEdgePointer(&playableEdge);
	enemies.push_back(enemy);
}

void Scene::createFlower(int posX, int posY) {
	Flor *flor = new Flor();
	flor->setEdgePointer(&playableEdge);
	flor->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, player);
	flor->setPosition(glm::vec2(posX * map->getTileSize(), posY * map->getTileSize()));
	flor->setTileMap(map);
	flowers.push_back(flor);
}

void Scene::createChest(int posX, int posY, ConsumableType type) {
	Chest *chest = new Chest();
	chest->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, type);
	chest->setPosition(glm::vec2(posX * map->getTileSize(), posY * map->getTileSize()));
	chest->setTileMap(map);
	chest->setEdgePointer(&playableEdge);
	chests.push_back(chest);
}

void Scene::createItem(int posX, int posY, ConsumableType type) {
	Consumable *item = new Consumable();
	item->setType(type);
	item->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	item->setPosition(glm::vec2(posX * map->getTileSize(), posY * map->getTileSize()));
	item->setTileMap(map);
	item->setEdgePointer(&playableEdge);
	items.push_back(item);
}

void Scene::createBarrel(int posX, int posY, bool breakable) {
	Barrel *barrel = new Barrel();
	barrel->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, breakable);
	barrel->setPosition(glm::vec2(posX * map->getTileSize(), posY * map->getTileSize()));
	barrel->setTileMap(map);
	barrel->setEdgePointer(&playableEdge);
	barrels.push_back(barrel);
}

void Scene::createBoss(int posX, int posY) {
	boss = new Boss();
	boss->setEdgePointer(&playableEdge);
	boss->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	boss->setPosition(glm::vec2(4 * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	boss->setTileMap(map);
}

void Scene::updateInteractions(Player *player, Enemy *enemy, bool projectile) {
	Direction dir = isCollision(player, enemy);

	if (dir == NONE) return;

	if ( (player->getAction() != PlayerAction::ATTACKING || dir != UP || projectile) && !god && !hit) {

		gui->setLives(gui->getLives() - 1);
		hit = true;
		hitTime = 0;

		if (dir == LEFT) {
			player->setHorizontalVelocity(-4);
			glm::ivec2 hitPosition = glm::ivec2(enemy->getPosition().x, player->getPosition().y) - glm::ivec2(player->getHitBox().x, 0);
			player->setPosition(hitPosition);
			return;
		}

		if (dir == RIGHT) {
			player->setHorizontalVelocity(4);
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

	if (player->getAction() == PlayerAction::ATTACKING && dir == UP) {
		gui->setScore(gui->getScore() + 100);
		glm::ivec2 hitPosition = glm::ivec2(player->getPosition().x, enemy->getPosition().y) - glm::ivec2(0, enemy->getHitBox().y);
		player->setPosition(hitPosition);
		player->setJump(-6);
		enemy->die();
	}
}

void Scene::updateInteractions(Player * player, Barrel * barrel) {

	Direction dir = isCollision(player, barrel);

	if (!interacting && Game::instance().getKey(GLFW_KEY_V)) {
		if (barrel->getState() == PICKED) {
			interacting = true;
			barrel->setState(THROWED);
			int direction = player->getVelocity().x > 0 ? 1 : -1;
			glm::vec2 newVelocity = glm::vec2(8*direction, -3);
			if (player->getVelocity().x == 0) {
				newVelocity.y = 0;
				newVelocity.x = 0;
			}
			barrel->setVelocity(newVelocity);
			player->setPicking(false);
		}
		else if (!player->isPicking() && barrel->getState() == FREE && dir != NONE) {
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

	else if (dir == UP && barrel->getState() == FREE) {
		glm::ivec2 newPosition = glm::ivec2(player->getPosition().x, barrel->getPosition().y - barrel->getHitBox().y);
		player->setPosition(newPosition);
		if (barrel->isExplosive() && player->getAction() == PlayerAction::ATTACKING) {
			player->setPosition(newPosition);
			player->setJump(-5);
			barrel->explode();
			return;
		}
		player->setAction(PlayerAction::GROUNDED);
		if (!player->playerInSurface())player->setStanding();
		//We must check if player needs to jump since the player update will make it think it is in the air
		if (Game::instance().getKey(GLFW_KEY_W)) {
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
		player->setJump(-7);
		Consumable *item = chest->open();
		item->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram); 
		item->setTileMap(map);
		item->drop(chest->getPosition());
		items.push_back(item);
		chest->die();
	}

	else if (dir == UP) {
		glm::ivec2 newPosition = glm::ivec2(player->getPosition().x, chest->getPosition().y - chest->getHitBox().y);
		player->setPosition(newPosition);
		player->setAction(PlayerAction::GROUNDED);
		if (!player->playerInSurface())player->setStanding();
		//We must check if player needs to jump since the player update will make it think it is in the air
		if (Game::instance().getKey(GLFW_KEY_W)) {
			player->setAction(PlayerAction::JUMPING);
			player->setVerticalVelocity(-7.0);
		}
	}
}

void Scene::updateInteractions(Player * player, Consumable * item) {

	Direction dir = isCollision(player, item);

	if (dir != NONE && item->getVelocity().y >= 0) {
		if (item->getType() == ConsumableType::POINTS) {
			gui->setScore(gui->getScore() + 200);
		}
		else {
			gui->setLives(min(4, gui->getLives()+1));
		}
		item->die();
	}
}

void Scene::checkKillCollision(Barrel * barrel, Entity * target) {
	Direction dir = isCollision(barrel, target);
	if (dir != NONE) {
		target->die();
		gui->setScore(gui->getScore() + 100);
		if (barrel->isExplosive()) barrel->explode();
	}
}

void Scene::update(int deltaTime) {

	currentTime += deltaTime;
	if (interacting) interacting = Game::instance().getKey(GLFW_KEY_V);
	if (interactingGod) interactingGod = Game::instance().getKey(GLFW_KEY_G);
	if (hit) hitTime += deltaTime;

	if (!interactingGod && Game::instance().getKey(GLFW_KEY_G)) {
		god = !god;
		interactingGod = true;
	}

	//Updates all entites and its collides with the player

	player->update(deltaTime);

	for (Enemy* enemy : enemies) {
		enemy->update(deltaTime);
		if (!enemy->isDying()) updateInteractions(player, enemy, false);
	}

	if (boss != nullptr) {
		boss->update(deltaTime);
		if (!boss->isDying()) updateInteractions(player, boss, false);
		checkShoots(player, boss);
	}

	for (Flor* flower : flowers) {
		flower->update(deltaTime);
		if (!flower->isDying()) updateInteractions(player, flower, false);
		if (flower->isShooting()) checkShoots(player, flower);
	}

	for (Barrel* barrel : barrels) {
		barrel->update(deltaTime);
		if (!barrel->hasExploded())updateInteractions(player, barrel);
	}

	for (Consumable* item : items) {
		item->update(deltaTime);
		if (!item->isDead()) updateInteractions(player, item);
	}

	for (Chest* chest : chests) {
		if (!chest->isDying())updateInteractions(player, chest);
		else chest->update(deltaTime);
	}

	//Check interaction between entities (basically checks if a barrel is hitting an enemy when throwed)

	for (Enemy* enemy : enemies) {
		for (Barrel* barrel : barrels) {
			if (barrel->getState() == THROWED && !enemy->isDying()) checkKillCollision(barrel, enemy);
		}
	}

	for (int i = 0; i < barrels.size(); i++) {
		for (int j = i+1; j < barrels.size(); j++) {
			if (barrels[i]->getState() == THROWED && !barrels[i]->hasExploded() && barrels[j]->isExplosive()) {
				Direction dir = isCollision(barrels[i], barrels[j]);
				if (dir != NONE) {
					if (barrels[i]->isExplosive()) barrels[i]->explode();
					barrels[j]->explode();
				}
			}
		}
	}

	for (Flor* flower : flowers) {
		for (Barrel* barrel : barrels) {
			if (barrel->getState() == THROWED && !flower->isDying()) checkKillCollision(barrel, flower);
		}
	}

	//Check if entities are dead to erase them so we do not update or render them anymore

	int n = enemies.size();
	for (int i = 0; i < n; i++) {
		if (enemies[i]->isDead()) {
			enemies.erase(enemies.begin() + i); 
			break;
		}
	}

	n = flowers.size();
	for (int i = 0; i < n; i++) {
		if (flowers[i]->isDead()) {
			flowers.erase(flowers.begin() + i); 
			break;
		}
	}

	n = chests.size();
	for (int i = 0; i < n; i++) {
		if (chests[i]->isDead()) {
			chests.erase(chests.begin() + i);
			break;
		}
	}

	n = barrels.size();
	for (int i = 0; i < n; i++) {
		if (barrels[i]->isDead()) {
			barrels.erase(barrels.begin() + i); 
			break;
		}
	}

	n = items.size();
	for (int i = 0; i < n; i++) {
		if (items[i]->isDead()) {
			items.erase(items.begin() + i); 
			break;
		}
	}

	if (Game::instance().getKey(GLFW_KEY_H)) gui->setLives(4);

	updateScreen(deltaTime);
	checkPlayerState();
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

	//background->render();

	map->render();

	for (Barrel* barrel : barrels) {
		barrel->render();
	}

	for (Chest* chest : chests) {
		chest->render();
	}

	for (Consumable* item : items) {
		if (!item->isDead()) item->render();
	}

	for (Enemy* enemy : enemies) {
		enemy->render();
	}

	for (Flor* flower : flowers) {
		flower->render();
	}

	if (boss != nullptr) boss->render();

	if (hit) {
		if (hitTime < 1000) {
			if (hitTime % 20 == 0) player->render();
		}
		else {
			hit = false;
			player->render();
		}
	}
	else player->render();
	gui->render();
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



