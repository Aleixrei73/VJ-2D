#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include "Entity.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.

enum class PlayerAction { JUMPING, GROUNDED, FALLING, ATTACKING, CROUCHING, CLIMBING};
enum PlayerAnims
{
	STAND_RIGHT, STAND_LEFT,
	PICK_RIGHT, PICK_LEFT,
	MOVE_RIGHT, MOVE_LEFT,
	ATTACK_RIGHT, ATTACK_LEFT,
	CROUCH_RIGHT, CROUCH_LEFT,
	JUMP_RIGHT, JUMP_LEFT,
	FALLING_RIGHT, FALLING_LEFT,
	FALLING_PICK_RIGHT, FALLING_PICK_LEFT,
	MOVE_PICK_RIGHT, MOVE_PICK_LEFT,
	JUMP_PICK_RIGHT, JUMP_PICK_LEFT,
	DRIFT_PICK_RIGHT, DRIFT_PICK_LEFT,
	DRIFT_RIGHT, DRIFT_LEFT,
	CLIMB
};


class Player : public Entity
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setJump(int vel);
	void setPicking(bool pick);
	void setAction(PlayerAction act);
	void setStanding();
	bool playerInSurface();
	bool isPicking();
	void die() override;
	bool isRight() const;

	PlayerAction getAction();
	
private:
	PlayerAction action;
	bool picking;
	bool holdingAttack;

	void setAnimation(PlayerAnims animation, int framesPerSec, int numberOfFrames,const glm::vec2 &texturePos);
	void loadAnimations();


};


#endif // _PLAYER_INCLUDE


