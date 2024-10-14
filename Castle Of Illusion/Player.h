#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include "Entity.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.

enum class PlayerAction { JUMPING, GROUNDED, FALLING, ATTACKING, CROUCHING};


class Player : public Entity
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setJump(int vel);

	PlayerAction getAction();
	
private:
	PlayerAction action;

};


#endif // _PLAYER_INCLUDE


