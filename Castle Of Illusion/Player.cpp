#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define MAX_VELOCITY 3


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	action = GROUNDED;
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
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	acceleration = glm::vec2(0.2, -2);
	velocity = glm::vec2(0, 0);
	
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);

	if (action == ATTACKING) {

		posPlayer.y += int(FALL_STEP*2);

		if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) action = GROUNDED;
	}

	else if (Game::instance().getKey(GLFW_KEY_S) && action == GROUNDED) {

		action = CROUCHING;

		velocity.x = 0;

		if (sprite->animation() == MOVE_LEFT) {
			sprite->changeAnimation(STAND_LEFT);
		}

		else if (sprite->animation() == MOVE_RIGHT) {
			sprite->changeAnimation(STAND_RIGHT);
		}
	}

	else if (Game::instance().getKey(GLFW_KEY_S)) {
		action = ATTACKING;
		velocity.x = 0;

	}

	else if(Game::instance().getKey(GLFW_KEY_A))	{

		if (sprite->animation() != MOVE_LEFT) {
			sprite->changeAnimation(MOVE_LEFT);
		}

		velocity.x = velocity.x - acceleration.x;

		if (velocity.x < -MAX_VELOCITY) velocity.x = -MAX_VELOCITY;

		posPlayer.x += int(velocity.x);

		if(map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32))) {
			posPlayer.x -= int(velocity.x);
			sprite->changeAnimation(STAND_LEFT);
		}

	}

	else if(Game::instance().getKey(GLFW_KEY_D)) {

		if (sprite->animation() != MOVE_RIGHT) {
			sprite->changeAnimation(MOVE_RIGHT);
		}

		velocity.x = velocity.x + acceleration.x;

		if (velocity.x > MAX_VELOCITY) velocity.x = MAX_VELOCITY;

		posPlayer.x += int(velocity.x);

		if(map->collisionMoveRight(posPlayer, glm::ivec2(32, 32))) {
			posPlayer.x -= int(velocity.x);
			sprite->changeAnimation(STAND_RIGHT);
		}

	}

	else {

		if (velocity.x < 0) {
			velocity.x = velocity.x + 0.1;
			if (velocity.x > 0) velocity.x = 0;
			posPlayer.x += int(velocity.x);
			if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32))) {
				posPlayer.x -= int(velocity.x);
				sprite->changeAnimation(STAND_LEFT);
			}
		}
		else if (velocity.x > 0) {
			velocity.x = velocity.x - 0.1;
			if (velocity.x < 0) velocity.x = 0;
			posPlayer.x += int(velocity.x);
			if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32))) {
				posPlayer.x -= int(velocity.x);
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
	
	if(action == JUMPING)
	{
		jumpAngle += JUMP_ANGLE_STEP;

		if(jumpAngle == 180) {
			action = FALLING;
			posPlayer.y = startY;
		}

		else {
			posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
			if (jumpAngle > 90) {
				if (!map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) action = JUMPING;
				else action = GROUNDED;
			}
		}

	}

	else if (action != ATTACKING){

		posPlayer.y += FALL_STEP;

		if (!map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) action = FALLING;
		else action = GROUNDED;

		if(action==GROUNDED) {

			if(Game::instance().getKey(GLFW_KEY_SPACE)) {
				action = JUMPING;
				jumpAngle = 0;
				startY = posPlayer.y;
			}

		}

	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}




