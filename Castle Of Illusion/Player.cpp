#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define MAX_FALL_VELOCITY 7
#define MAX_VELOCITY 3


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, 
	MOVE_LEFT, MOVE_RIGHT, 
	ATTACK_LEFT, ATTACK_RIGHT,
	CROUCH_LEFT, CROUCH_RIGHT,
	JUMP_LEFT, JUMP_RIGHT, 
	THROW_LEFT, THROW_RIGHT,
	PICK_LEFT, PICK_RIGHT,
	MOVE_PICK_LEFT, MOVE_PICK_RIGHT,
	JUMP_PICK_LEFT, JUMP_PICK_RIGHT,
	DRIFT_LEFT, DRIFT_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	float sprite_x_space = 1.f / 15;
	float sprite_y_space = 1.f / 14;

	action = PlayerAction::GROUNDED;
	spritesheet.loadFromFile("images/player.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 36), glm::vec2(1.f/15, 1.f / 14), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(20);
	
	float spriteShift_x = 0;
	float spriteShift_y = 0;

		sprite->setAnimationSpeed(STAND_RIGHT, 2);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(sprite_x_space, 0.f));
		
		sprite->setAnimationSpeed(STAND_LEFT, 2);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.5f));
		sprite->addKeyframe(STAND_LEFT, glm::vec2(sprite_x_space, 0.5f));

		spriteShift_x = 2*sprite_x_space;
		spriteShift_y = 0;

		sprite->setAnimationSpeed(MOVE_RIGHT, 8);

		for (int i = 1; i < 8; i++) {
			sprite->addKeyframe(MOVE_RIGHT, glm::vec2(spriteShift_x + sprite_x_space*i, spriteShift_y));
		}

		sprite->setAnimationSpeed(MOVE_LEFT, 8);

		for (int i = 0; i < 8; i++) {
			sprite->addKeyframe(MOVE_LEFT, glm::vec2(spriteShift_x + sprite_x_space*i, 0.5f + spriteShift_y));
		}

		sprite->setAnimationSpeed(ATTACK_RIGHT, 8);

		spriteShift_x = 5 * sprite_x_space;
		spriteShift_y = sprite_y_space;

		for (int i = 0; i < 2; i++) {
			sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(spriteShift_x + sprite_x_space*i, spriteShift_y));
		}

		sprite->setAnimationSpeed(ATTACK_LEFT, 8);

		for (int i = 0; i < 2; i++) {
			sprite->addKeyframe(ATTACK_LEFT, glm::vec2(spriteShift_x + sprite_x_space*i, 0.5f + spriteShift_y));
		}

		sprite->setAnimationSpeed(CROUCH_RIGHT, 8);

		spriteShift_x =0;
		spriteShift_y = sprite_y_space;

		for (int i = 0; i < 2; i++) {
			sprite->addKeyframe(CROUCH_RIGHT, glm::vec2(spriteShift_x + sprite_x_space*i, spriteShift_y));
		}

		sprite->setAnimationSpeed(CROUCH_LEFT, 8);

		for (int i = 0; i < 2; i++) {
			sprite->addKeyframe(CROUCH_LEFT, glm::vec2(spriteShift_x + sprite_x_space*i, 0.5f + spriteShift_y));
		}

		sprite->setAnimationSpeed(JUMP_RIGHT, 8);

		spriteShift_x = sprite_x_space*2;
		spriteShift_y = sprite_y_space;

		for (int i = 0; i < 1; i++) {
			sprite->addKeyframe(JUMP_RIGHT, glm::vec2(spriteShift_x + sprite_x_space*i, spriteShift_y));
		}

		sprite->setAnimationSpeed(JUMP_LEFT, 8);

		for (int i = 0; i < 1; i++) {
			sprite->addKeyframe(JUMP_LEFT, glm::vec2(spriteShift_x + sprite_x_space*i, 0.5f + spriteShift_y));
		}



		
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
	acceleration = glm::vec2(0.2,0.2);
	velocity = glm::vec2(0, 0);
	hitBox = glm::ivec2(32, 32);
	
}

void Player::update(int deltaTime)
{

	if (position.y >= *mapEdge) {
		dying = true;
	}

	if (dying) {
		velocity.y += acceleration.y *deltaTime / 10;
		position.y += int(velocity.y);
		if (position.y - hitBox.y > *mapEdge) {
			death = true;
			dying = false;
		}
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
		return;
	}


	sprite->update(deltaTime);
	if (holdingAttack) holdingAttack = Game::instance().getKey(GLFW_KEY_C);

	hitBox.y = 32;

	if (action == PlayerAction::ATTACKING) {

		velocity.y = velocity.y + acceleration.y * 3;

		if (velocity.y > (MAX_FALL_VELOCITY + 3)) velocity.y = MAX_FALL_VELOCITY + 3;

		position.y += int(velocity.y);

		if (map->collisionMoveDown(position, hitBox, &position.y)) {
			action = PlayerAction::GROUNDED;
			velocity.y = 0;
		}
	}

	else if (Game::instance().getKey(GLFW_KEY_S) && action == PlayerAction::GROUNDED) {

		hitBox.y = 16;

		if (sprite->animation() == MOVE_LEFT) {
			sprite->changeAnimation(STAND_LEFT);
		}

		else if (sprite->animation() == MOVE_RIGHT) {
			sprite->changeAnimation(STAND_RIGHT);
		}
	}

	else if (!holdingAttack && (action == PlayerAction::FALLING || action == PlayerAction::JUMPING) && Game::instance().getKey(GLFW_KEY_C)) {
		holdingAttack = true;
		action = PlayerAction::ATTACKING;
		velocity.y = 0;

	}

	if(Game::instance().getKey(GLFW_KEY_A))	{

		if (sprite->animation() != MOVE_LEFT) {
			sprite->changeAnimation(MOVE_LEFT);
		}

		velocity.x = velocity.x - acceleration.x * deltaTime/10;

		if (velocity.x < -MAX_VELOCITY) velocity.x = -MAX_VELOCITY;

		position.x += int(velocity.x);

		if(map->collisionMoveLeft(position, hitBox)) {
			position.x -= int(velocity.x);
			velocity.x = 0;
			sprite->changeAnimation(STAND_LEFT);
		}

	}

	else if(Game::instance().getKey(GLFW_KEY_D)) {

		if (sprite->animation() != MOVE_RIGHT) {
			sprite->changeAnimation(MOVE_RIGHT);
		}

		velocity.x = velocity.x + acceleration.x * deltaTime/10;

		if (velocity.x > MAX_VELOCITY) velocity.x = MAX_VELOCITY;

		position.x += int(velocity.x);

		if(map->collisionMoveRight(position, hitBox)) {
			position.x -= int(velocity.x);
			velocity.x = 0;
			sprite->changeAnimation(STAND_RIGHT);
		}

	}

	else {

		if (velocity.x < 0) {
			velocity.x = velocity.x + float(0.1) * deltaTime / 10;
			if (velocity.x > 0) velocity.x = 0;
			position.x += int(velocity.x);
			if (map->collisionMoveLeft(position, hitBox)) {
				position.x -= int(velocity.x);
				velocity.x = 0;
				sprite->changeAnimation(STAND_LEFT);
			}
		}
		else if (velocity.x > 0) {
			velocity.x = velocity.x - float(0.1) * deltaTime / 10;
			if (velocity.x < 0) velocity.x = 0;
			position.x += int(velocity.x);
			if (map->collisionMoveRight(position, hitBox)) {
				position.x -= int(velocity.x);
				velocity.x = 0;
				sprite->changeAnimation(STAND_RIGHT);
			}
		}


		if (sprite->animation() == MOVE_LEFT) {
			sprite->changeAnimation(STAND_LEFT);
		}

		else if (sprite->animation() == MOVE_RIGHT) {
			sprite->changeAnimation(STAND_RIGHT);
		}

	}
	
	if(action == PlayerAction::JUMPING)
	{
		velocity.y += acceleration.y*deltaTime/10;

		position.y += int(velocity.y);

		if (velocity.y > 0) {
			action = PlayerAction::FALLING;
			if (map->collisionMoveDown(position, hitBox, &position.y)) action = PlayerAction::GROUNDED;
		}

	}

	else if (action == PlayerAction::FALLING) {
		
		velocity.y += acceleration.y*deltaTime/10;

		if (velocity.y > MAX_FALL_VELOCITY) velocity.y = MAX_FALL_VELOCITY;

		position.y += int(velocity.y);

		if (map->collisionMoveDown(position, hitBox, &position.y)) {
			action = PlayerAction::GROUNDED;
			velocity.y = 0;
		}

	}

	else if (action != PlayerAction::ATTACKING){

		velocity.y = 0;

		position.y += 1;

 		if (!map->collisionMoveDown(position, hitBox, &position.y)) action = PlayerAction::FALLING;
		else action = PlayerAction::GROUNDED;

		if(action == PlayerAction::GROUNDED) {
			 
			if(Game::instance().getKey(GLFW_KEY_SPACE)) {
				action = PlayerAction::JUMPING;
				velocity.y = -MAX_FALL_VELOCITY;
			}

		}

	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));

}

void Player::render()
{
	sprite->render();
}

void Player::setJump(int vel) {
	action = PlayerAction::FALLING;
	velocity.y = vel;
}

void Player::setPicking(bool pick) {
	picking = pick;
}

void Player::setAction(PlayerAction act) {
	action = act;
}

bool Player::isPicking() {
	return picking;
}

void Player::die() {
	dying = true;
	velocity.y = -7;
}

PlayerAction Player::getAction()
{
	return action;
}




