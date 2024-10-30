#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define MAX_FALL_VELOCITY 7
#define MAX_VELOCITY 3


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{

	action = PlayerAction::GROUNDED;
	spritesheet.loadFromFile("images/player.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 36), glm::vec2(1.f/15, 1.f / 14), &spritesheet, &shaderProgram);

	loadAnimations();
			
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
	acceleration = glm::vec2(0.1,0.2);
	velocity = glm::vec2(0, 0);
	hitBox = glm::ivec2(32, 32);
	
}

void Player::update(int deltaTime) {

	int posLadder = map->collisionLadderDown(position, hitBox);

	if (posLadder != -1 && Game::instance().getKey(GLFW_KEY_S)) {
		velocity.x = 0;
		velocity.y = 0;
		if (!(action == PlayerAction::CLIMBING)) {
			sprite->changeAnimation(CLIMB);
			position.x = posLadder*map->getTileSize();
			position.y += 16;
			action = PlayerAction::CLIMBING;
			sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
			sprite->update(deltaTime);
			return;
		}
		else {
			position.y += 1;
			sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
			sprite->update(deltaTime);
			return;
		}
	}

	else if (posLadder != -1 && Game::instance().getKey(GLFW_KEY_W)) {
		position.y -= 1;
		position.x = posLadder * map->getTileSize();
		action = PlayerAction::CLIMBING;
		velocity.x = 0;
		velocity.y = 0;
		if (sprite->animation() != CLIMB) sprite->changeAnimation(CLIMB);
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
		sprite->update(deltaTime);
		return;
	}

	else if (action == PlayerAction::CLIMBING) {
		if (posLadder == -1) action == PlayerAction::FALLING;
		else return;
	}

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
	if (holdingAttack) holdingAttack = Game::instance().getKey(GLFW_KEY_S);

	hitBox.y = 32;

	if (action == PlayerAction::ATTACKING) {

		velocity.y = velocity.y + acceleration.y;

		if (velocity.y > (MAX_FALL_VELOCITY)) velocity.y = MAX_FALL_VELOCITY;

		position.y += int(velocity.y);

		if (map->collisionMoveDown(position, hitBox, &position.y)) {
			action = PlayerAction::GROUNDED;
			velocity.y = 0;
		}
	}

	else if (!holdingAttack && (action == PlayerAction::FALLING || action == PlayerAction::JUMPING) && Game::instance().getKey(GLFW_KEY_S)) {
		holdingAttack = true;
		action = PlayerAction::ATTACKING;
		velocity.y = 0;
		sprite->changeAnimation(ATTACK_RIGHT);

	}

	if(Game::instance().getKey(GLFW_KEY_A))	{

		if (action == PlayerAction::ATTACKING) {
			if (sprite->animation() != ATTACK_LEFT) sprite->changeAnimation(ATTACK_LEFT);
		}

		else if (action == PlayerAction::CROUCHING) {
			if (sprite->animation() != CROUCH_LEFT) sprite->changeAnimation(CROUCH_LEFT);
		}

		else if (action == PlayerAction::JUMPING) {

			if (picking  && sprite->animation() != JUMP_PICK_LEFT) sprite->changeAnimation(JUMP_PICK_LEFT);
			else if (!picking && sprite->animation() != JUMP_LEFT) sprite->changeAnimation(JUMP_LEFT);

		}

		else if (action == PlayerAction::FALLING) {

			if (picking  && sprite->animation() != FALLING_PICK_LEFT) sprite->changeAnimation(FALLING_PICK_LEFT);
			else if (!picking && sprite->animation() != FALLING_LEFT) sprite->changeAnimation(FALLING_LEFT);

		}

		else if (picking) {
			if (sprite->animation() != MOVE_PICK_LEFT) sprite->changeAnimation(MOVE_PICK_LEFT);
		}

		else if (sprite->animation() != MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);

		if (velocity.x == 0 && action != PlayerAction::CROUCHING) velocity.x = -1;

		velocity.x = velocity.x - acceleration.x * deltaTime/10;

		if (velocity.x < -MAX_VELOCITY) velocity.x = -MAX_VELOCITY;

		position.x += int(velocity.x);

		if(map->collisionMoveLeft(position, hitBox)) {
			position.x -= int(velocity.x);
			velocity.x = 0;
			if (action == PlayerAction::GROUNDED) {
				if (picking) sprite->changeAnimation(PICK_LEFT);
				else sprite->changeAnimation(STAND_LEFT);
			}
		}

	}

	else if(Game::instance().getKey(GLFW_KEY_D)) {

		if (action == PlayerAction::ATTACKING) {
			if (sprite->animation() != ATTACK_RIGHT) sprite->changeAnimation(ATTACK_RIGHT);
		}

		else if (action == PlayerAction::CROUCHING) {
			if(sprite->animation() != CROUCH_RIGHT) sprite->changeAnimation(CROUCH_RIGHT);
		}

		else if (action == PlayerAction::JUMPING) {

			if (picking  && sprite->animation() != JUMP_PICK_RIGHT) sprite->changeAnimation(JUMP_PICK_RIGHT);
			else if (!picking && sprite->animation() != JUMP_RIGHT) sprite->changeAnimation(JUMP_RIGHT);

		}

		else if (action == PlayerAction::FALLING) {

			if (picking  && sprite->animation() != FALLING_PICK_RIGHT) sprite->changeAnimation(FALLING_PICK_RIGHT);
			else if (!picking && sprite->animation() != FALLING_RIGHT) sprite->changeAnimation(FALLING_RIGHT);

		}

		else if (picking) {
			if (sprite->animation() != MOVE_PICK_RIGHT) sprite->changeAnimation(MOVE_PICK_RIGHT);
		}

		else if (sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);

		if (velocity.x == 0 && action != PlayerAction::CROUCHING) velocity.x = 1;

		velocity.x = velocity.x + acceleration.x * deltaTime/10;

		if (velocity.x > MAX_VELOCITY) velocity.x = MAX_VELOCITY;

		position.x += int(velocity.x);

		if(map->collisionMoveRight(position, hitBox)) {
			position.x -= int(velocity.x);
			velocity.x = 0;
			if (action == PlayerAction::GROUNDED) {
				if (picking) sprite->changeAnimation(PICK_RIGHT);
				else sprite->changeAnimation(STAND_RIGHT);
			}
		}

	}

	else {

		if (velocity.x < 0) {

			velocity.x = velocity.x + float(0.1) * deltaTime / 10;
			if (velocity.x >= 0) {
				velocity.x = 0;
				if (!(action == PlayerAction::JUMPING || action == PlayerAction::FALLING || action == PlayerAction::ATTACKING)) {
					if (picking) sprite->changeAnimation(PICK_LEFT);
					else sprite->changeAnimation(STAND_LEFT);
				}
			}
			position.x += int(velocity.x);
			if (map->collisionMoveLeft(position, hitBox)) {
				position.x -= int(velocity.x);
				velocity.x = 0;
				if (action == PlayerAction::GROUNDED) {
					if (picking) sprite->changeAnimation(PICK_LEFT);
					else sprite->changeAnimation(STAND_LEFT);
				}
			}
		}
		else if (velocity.x > 0) {
			velocity.x = velocity.x - float(0.1) * deltaTime / 10;
			if (velocity.x <= 0) {
				velocity.x = 0;
				if (!(action == PlayerAction::JUMPING || action == PlayerAction::FALLING || action == PlayerAction::ATTACKING)) {
					if (picking) sprite->changeAnimation(PICK_RIGHT);
					else sprite->changeAnimation(STAND_RIGHT);
				}
			}
			position.x += int(velocity.x);
			if (map->collisionMoveRight(position, hitBox)) {
				position.x -= int(velocity.x);
				velocity.x = 0;
				if (action == PlayerAction::GROUNDED) {
					if (picking) sprite->changeAnimation(PICK_RIGHT);
					else sprite->changeAnimation(STAND_RIGHT);
				}
			}
		}


		if (sprite->animation() == MOVE_LEFT) {
			sprite->changeAnimation(DRIFT_LEFT);
		}

		else if (sprite->animation() == MOVE_RIGHT) {
			sprite->changeAnimation(DRIFT_RIGHT);
		}

		else if (sprite->animation() == MOVE_PICK_RIGHT) {
			sprite->changeAnimation(DRIFT_PICK_RIGHT);
		}

		else if (sprite->animation() == MOVE_PICK_LEFT) {
			sprite->changeAnimation(DRIFT_PICK_LEFT);
		}


	}
	
	if(action == PlayerAction::JUMPING)
	{

		int anim = sprite->animation();

		if (picking) {
			if (anim % 2 == 0 && sprite->animation() != JUMP_PICK_RIGHT) sprite->changeAnimation(JUMP_PICK_RIGHT);
			else if (anim % 2 != 0 && sprite->animation() != JUMP_PICK_LEFT) sprite->changeAnimation(JUMP_PICK_LEFT);
		}
		else {
			if (anim % 2 == 0 && sprite->animation() != JUMP_RIGHT) sprite->changeAnimation(JUMP_RIGHT);
			else if (anim % 2 != 0 && sprite->animation() != JUMP_LEFT) sprite->changeAnimation(JUMP_LEFT);
		}

		velocity.y += acceleration.y*deltaTime/10;

		position.y += int(velocity.y);

		if (velocity.y > 0) {
			action = PlayerAction::FALLING;
			if (map->collisionMoveDown(position, hitBox, &position.y)) action = PlayerAction::GROUNDED;
		}

	}

	else if (action == PlayerAction::FALLING) {

		int anim = sprite->animation();

		if (anim < 12 || anim > 15) {
			if (anim % 2 == 0) {
				if (picking) sprite->changeAnimation(FALLING_PICK_RIGHT);
				else sprite->changeAnimation(FALLING_RIGHT);
			}
			else {
				if (picking) sprite->changeAnimation(FALLING_PICK_LEFT);
				else sprite->changeAnimation(FALLING_LEFT);
			}
		}
		
		velocity.y += acceleration.y*deltaTime/10;

		if (velocity.y > MAX_FALL_VELOCITY) velocity.y = MAX_FALL_VELOCITY;

		position.y += int(velocity.y);

		if (map->collisionMoveDown(position, hitBox, &position.y)) {
			action = PlayerAction::GROUNDED;
			velocity.y = 0;
		}

	}

	else if (action != PlayerAction::ATTACKING) {

		velocity.y = 0;

		position.y += 1;

 		if (!map->collisionMoveDown(position, hitBox, &position.y)) action = PlayerAction::FALLING;
		else action = PlayerAction::GROUNDED;

		if(action == PlayerAction::GROUNDED) {

			int animation = sprite->animation();

			if (!holdingAttack && Game::instance().getKey(GLFW_KEY_S)) {
				hitBox.y = 16;
				velocity.x = 0;
				velocity.y = 0;

				action = PlayerAction::CROUCHING;

				int anim = sprite->animation();

				if (anim % 2 == 0) {
					if (anim != CROUCH_RIGHT) sprite->changeAnimation(CROUCH_RIGHT);
				}

				else if (anim != CROUCH_LEFT) {
					sprite->changeAnimation(CROUCH_LEFT);
				}
			}
			 
			else if(Game::instance().getKey(GLFW_KEY_W)) {
				SoundManager::instance().playSound(SoundType::JUMP);
				action = PlayerAction::JUMPING;
				velocity.y = -7.5;

				if (animation % 2 == 0) {
					if (picking) sprite->changeAnimation(JUMP_PICK_RIGHT);
					else sprite->changeAnimation(JUMP_RIGHT);
				}

				else {
					if (picking) sprite->changeAnimation(JUMP_PICK_LEFT);
					else sprite->changeAnimation(JUMP_LEFT);
				}

			}

			else if (velocity.x == 0 && animation > 3) {

				if (animation % 2 == 0) {
					if (picking) sprite->changeAnimation(PICK_RIGHT);
					else sprite->changeAnimation(STAND_RIGHT);
				}
				else {
					if (picking) sprite->changeAnimation(PICK_LEFT);
					else sprite->changeAnimation(STAND_LEFT);
				}
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
	if (!(action==PlayerAction::ATTACKING))action = PlayerAction::JUMPING;
	velocity.y = vel;
}

void Player::setPicking(bool pick) {
	picking = pick;
}

void Player::setAction(PlayerAction act) {
	action = act;
}

void Player::setStanding() {

	int anim = sprite->animation();

	if (picking) {
		if ( anim%2 == 0 && sprite->animation() != PICK_RIGHT) sprite->changeAnimation(PICK_RIGHT);
		else if (anim % 2 != 0 && sprite->animation() != PICK_LEFT) sprite->changeAnimation(PICK_LEFT);
	}
	else {
		if (anim % 2 == 0 && sprite->animation() != STAND_RIGHT) sprite->changeAnimation(STAND_RIGHT);
		else if (anim % 2 != 0 && sprite->animation() != STAND_LEFT) sprite->changeAnimation(STAND_LEFT);
	}
}

bool Player::playerInSurface() {
	int anim = sprite->animation();
	bool surfaced = anim == MOVE_RIGHT || anim == MOVE_LEFT || anim == STAND_RIGHT || anim == STAND_LEFT || anim==DRIFT_LEFT ||anim==DRIFT_RIGHT;
	surfaced = surfaced || anim == PICK_LEFT || anim == PICK_RIGHT || anim == MOVE_PICK_LEFT || anim == MOVE_PICK_RIGHT ||anim==DRIFT_PICK_LEFT ||anim==DRIFT_PICK_RIGHT;
	return surfaced;
}

bool Player::isPicking() {
	return picking;
}

void Player::die() {
	dying = true;
	velocity.y = -7;
}

bool Player::isRight() const
{
	return sprite->animation()%2 == 0;
}

PlayerAction Player::getAction()
{
	return action;
}

void Player::setAnimation(PlayerAnims animation, int framesPerSec, int numberOfFrames, const glm::vec2 &texturePos) {

	sprite->setAnimationSpeed(animation, framesPerSec);

	float sprite_x_space = 1.f / 15;
	float sprite_y_space = 1.f / 14;

	for (int i = 0; i < numberOfFrames; i++) {
		sprite->addKeyframe(animation, glm::vec2(texturePos.x + sprite_x_space*i, texturePos.y));
	}

	int nextAnim = animation + 1;

	sprite->setAnimationSpeed(nextAnim, framesPerSec);

	for (int i = 0; i < numberOfFrames; i++) {
		sprite->addKeyframe(nextAnim, glm::vec2(texturePos.x + sprite_x_space*i, 0.5f + texturePos.y));
	}

}

void Player::loadAnimations() {

	float sprite_x_space = 1.f / 15;
	float sprite_y_space = 1.f / 14;

	sprite->setNumberAnimations(26);

	float spriteShift_x = 0;
	float spriteShift_y = 0;

	setAnimation(STAND_RIGHT, 2, 2, glm::vec2(spriteShift_x, spriteShift_y));

	spriteShift_x = 2 * sprite_x_space;
	spriteShift_y = 0;

	setAnimation(MOVE_RIGHT, 8, 6, glm::vec2(spriteShift_x, spriteShift_y));

	spriteShift_x = 5 * sprite_x_space;
	spriteShift_y = sprite_y_space;

	setAnimation(ATTACK_RIGHT, 8, 2, glm::vec2(spriteShift_x, spriteShift_y));

	spriteShift_x = 0;
	spriteShift_y = sprite_y_space;

	setAnimation(CROUCH_RIGHT, 8, 2, glm::vec2(spriteShift_x, spriteShift_y));

	spriteShift_x = sprite_x_space * 2;
	spriteShift_y = sprite_y_space;

	setAnimation(JUMP_RIGHT, 8, 1, glm::vec2(spriteShift_x, spriteShift_y));

	spriteShift_x = sprite_x_space * 3;
	spriteShift_y = sprite_y_space;

	setAnimation(FALLING_RIGHT, 8, 1, glm::vec2(spriteShift_x, spriteShift_y));

	spriteShift_x = sprite_x_space * 4;
	spriteShift_y = sprite_y_space * 3;

	setAnimation(PICK_RIGHT, 3, 2, glm::vec2(spriteShift_x, spriteShift_y));

	spriteShift_x = 0;
	spriteShift_y = sprite_y_space * 4;

	setAnimation(MOVE_PICK_RIGHT, 8, 8, glm::vec2(spriteShift_x, spriteShift_y));

	spriteShift_x = sprite_x_space * 8;
	spriteShift_y = sprite_y_space * 4;

	setAnimation(JUMP_PICK_RIGHT, 3, 1, glm::vec2(spriteShift_x, spriteShift_y));

	spriteShift_x = sprite_x_space * 9;
	spriteShift_y = sprite_y_space * 4;

	setAnimation(FALLING_PICK_RIGHT, 3, 1, glm::vec2(spriteShift_x, spriteShift_y));

	spriteShift_x = sprite_x_space * 7;
	spriteShift_y = sprite_y_space * 3;

	setAnimation(DRIFT_PICK_RIGHT, 3, 2, glm::vec2(spriteShift_x, spriteShift_y));

	spriteShift_x = sprite_x_space * 10;
	spriteShift_y = 0;

	setAnimation(DRIFT_RIGHT, 3, 1, glm::vec2(spriteShift_x, spriteShift_y));

	spriteShift_x = sprite_x_space * 12;
	spriteShift_y = 0;

	setAnimation(CLIMB, 6, 2, glm::vec2(spriteShift_x, spriteShift_y));


}




