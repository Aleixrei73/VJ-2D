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
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
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

		if (velocity.y > FALL_STEP * 2) velocity.y = FALL_STEP * 2;

		posPlayer.y += int(velocity.y);

		if (map->collisionMoveDown(posPlayer, hitBox, &posPlayer.y)) action = PlayerAction::GROUNDED;
	}

	else if (Game::instance().getKey(GLFW_KEY_S) && action == PlayerAction::GROUNDED) {

		hitBox.y = 16;

		velocity.x = 0;

		if (sprite->animation() == MOVE_LEFT) {
			sprite->changeAnimation(STAND_LEFT);
		}

		else if (sprite->animation() == MOVE_RIGHT) {
			sprite->changeAnimation(STAND_RIGHT);
		}
	}

	else if (Game::instance().getKey(GLFW_KEY_S)) {
		action = PlayerAction::ATTACKING;
		velocity.x = 0;
		velocity.y = 0;

	}

	else if(Game::instance().getKey(GLFW_KEY_A))	{

		if (sprite->animation() != MOVE_LEFT) {
			sprite->changeAnimation(MOVE_LEFT);
		}

		velocity.x = velocity.x - acceleration.x;

		if (velocity.x < -MAX_VELOCITY) velocity.x = -MAX_VELOCITY;

		posPlayer.x += int(velocity.x);

		if(map->collisionMoveLeft(posPlayer, hitBox)) {
			posPlayer.x -= int(velocity.x);
			velocity.x = 0;
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

		if(map->collisionMoveRight(posPlayer, hitBox)) {
			posPlayer.x -= int(velocity.x);
			velocity.x = 0;
			sprite->changeAnimation(STAND_RIGHT);
		}

	}

	else {

		if (velocity.x < 0) {
			velocity.x = velocity.x + 0.1;
			if (velocity.x > 0) velocity.x = 0;
			posPlayer.x += int(velocity.x);
			if (map->collisionMoveLeft(posPlayer, hitBox)) {
				posPlayer.x -= int(velocity.x);
				velocity.x = 0;
				sprite->changeAnimation(STAND_LEFT);
			}
		}
		else if (velocity.x > 0) {
			velocity.x = velocity.x - 0.1;
			if (velocity.x < 0) velocity.x = 0;
			posPlayer.x += int(velocity.x);
			if (map->collisionMoveRight(posPlayer, hitBox)) {
				posPlayer.x -= int(velocity.x);
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
		jumpAngle += JUMP_ANGLE_STEP;

		if(jumpAngle == 180) {
			action = PlayerAction::FALLING;
			posPlayer.y = startY;
		}

		else {
			posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
			if (jumpAngle > 90) {
				if (!map->collisionMoveDown(posPlayer, hitBox, &posPlayer.y)) action = PlayerAction::JUMPING;
				else action = PlayerAction::GROUNDED;
			}
		}

	}

	else if (action != PlayerAction::ATTACKING){

		posPlayer.y += FALL_STEP;

		if (!map->collisionMoveDown(posPlayer, hitBox, &posPlayer.y)) action = PlayerAction::FALLING;
		else action = PlayerAction::GROUNDED;

		if(action== PlayerAction::GROUNDED) {

			if(Game::instance().getKey(GLFW_KEY_SPACE)) {
				action = PlayerAction::JUMPING;
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

void Player::setHorizontalVelocity(float vel) {
	velocity.x = vel;
}

void Player::setVerticalVelocity(const float & vel) {
	velocity.y = vel;
}

void Player::startJump(int angle) {
	action = PlayerAction::JUMPING;
	jumpAngle = 0;
	startY = posPlayer.y;
}

glm::ivec2 Player::getHitBox()
{
	return hitBox;
}

glm::ivec2 Player::getPosition()
{
	return posPlayer;
}

PlayerAction Player::getAction()
{
	return action;
}




