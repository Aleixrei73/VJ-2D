#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "Barrel.h"
#include "Chest.h"
#include "Consumable.h"
#include "Background.h"
#include "GUI.h"
#include "Flor.h"
#include "Boss.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

enum Direction { NONE = 0, LEFT, RIGHT, UP};

class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void setLevel(string lvl);
	void update(int deltaTime);
	void render();
	void startPreparation();

private:
	void initShaders();
	void updateInteractions(Player * player, Enemy * enemy, bool projectile);
	void updateInteractions(Player * player, Barrel * barrel);
	void updateInteractions(Player * player, Chest * chest);
	void updateInteractions(Player * player, Consumable * item);
	void checkKillCollision(Barrel * barrel, Enemy * target);
	void checkKillCollision(Barrel * barrel, Chest * target);
	Direction isCollision(Entity * player, Entity * enemy);
	void updateScreen(int deltaTime);
	void checkPlayerState();
	void checkShoots(Player *player, Flor* flower);
	void checkShoots(Player* player, Boss* boss);
	void restart();
	void deleteEntities();
	void initEntities();

	void createEnemy(int posX, int posY);
	void createFlower(int posX, int posY);
	void createChest(int posX, int posY, ConsumableType type);
	void createItem(int posX, int posY, ConsumableType type);
	void createBarrel(int posX, int posY, bool breakable);
	void createBoss(int posX, int posY);

private:
	TileMap *map;
	string level;
	Player *player;
	Background *background;
	GUI *gui;
	Boss* boss;
	vector<Enemy*> enemies;
	vector<Barrel*> barrels;
	vector<Chest*> chests;
	vector<Consumable *> items;
	vector<Flor*> flowers;
	ShaderProgram texProgram;
	float currentTime;
	int hitTime;
	bool hit;
	glm::ivec2 cameraPos;
	glm::mat4 projection;
	bool interacting, god, interactingGod;
	int playableEdge;
	int height;
	bool passed;

};


#endif // _SCENE_INCLUDE

