#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"


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
	Direction checkCollision(Player * player, Enemy * enemy);

private:
	TileMap *map;
	Player *player;
	Enemy *enemy;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

};


#endif // _SCENE_INCLUDE

