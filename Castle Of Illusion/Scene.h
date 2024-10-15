#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "Barrel.h"


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
	Direction checkCollisionDirection(Player * player, Enemy * enemy);
	bool isCollision(Entity *a, Entity *b);

private:
	TileMap *map;
	Player *player;
	Enemy *enemy;
	Barrel *barrel;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

};


#endif // _SCENE_INCLUDE

