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
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	action = PlayerAction::GROUNDED;
	spritesheet.loadFromFile("images/bub.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);
	
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.f));
		
		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.25f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.5f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
	acceleration = glm::vec2(0.2,0.2);
	velocity = glm::vec2(0, 0);
	hitBox = glm::ivec2(32, 32);
	
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);

	hitBox.y = 32;

	if (action == PlayerAction::ATTACKING) {

		velocity.y = velocity.y + acceleration.y * 5;

		if (velocity.y > (MAX_FALL_VELOCITY + 3)) velocity.y = MAX_FALL_VELOCITY + 3;

		position.y += int(velocity.y);

		if (map->collisionMoveDown(position, hitBox, &position.y)) {
			action = PlayerAction::GROUNDED;
			velocity.y = 0;
		}
	}

	else if (Game::instance().getKey(GLFW_KEY_S) && action == PlayerAction::GROUNDED) {

		hitBox.y = 16;

		velocity.x = 0;
		velocity.y = 0;

		if (sprite->animation() == MOVE_LEFT) {
			sprite->changeAnimation(STAND_LEFT);
		}

		else if (sprite->animation() == MOVE_RIGHT) {
			sprite->changeAnimation(STAND_RIGHT);
		}
	}

	else if (Game::instance().getKey(GLFW_KEY_S)) {
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

		if (velocity.y >= MAX_FALL_VELOCITY) velocity.y = MAX_FALL_VELOCITY;

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

PlayerAction Player::getAction()
{
	return action;
}




