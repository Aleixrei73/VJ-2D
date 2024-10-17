#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "Barrel.h"
#include "Chest.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.

enum Direction { LEFT, RIGHT, UP, NONE };


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
	Direction checkCollisionDirection(Player * player, Enemy * enemy);
	bool isCollision(Entity *a, Entity *b);

private:
	TileMap *map;
	Player *player;
	Enemy *enemy;
	Barrel *barrel;
	Chest *chest;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	bool interacting;
};


#endif // _SCENE_INCLUDE

