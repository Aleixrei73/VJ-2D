#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "Barrel.h"
#include "Chest.h"
#include "Consumable.h"
#include "Background.h"
#include "GUI.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.

enum Direction { NONE = 0, LEFT, RIGHT, UP};


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();
	void updateInteractions(Player * player, Enemy * enemy);
	void updateInteractions(Player * player, Barrel * barrel);
	void updateInteractions(Player * player, Chest * chest);
	void updateInteractions(Player * player, Consumable * item);
	void checkKillCollision(Entity * killer, Entity * target);
	Direction isCollision(Entity * player, Entity * enemy);
	void updateScreen(int deltaTime);
	void checkPlayerState();

private:
	TileMap *map;
	Player *player;
	Background *background;
	GUI *gui;
	vector<Enemy*> enemies;
	vector<Barrel*> barrels;
	vector<Chest*> chests;
	vector<Consumable *> items;
	ShaderProgram texProgram;
	float currentTime;
	glm::ivec2 cameraPos;
	glm::mat4 projection;
	bool interacting, god, interactingGod;
	int playableEdge;

};


#endif // _SCENE_INCLUDE

