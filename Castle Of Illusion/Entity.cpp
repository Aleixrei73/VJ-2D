#include "Entity.h"

void Entity::setTileMap(TileMap * tileMap)
{
	map = tileMap;
}

void Entity::setVelocity(const glm::vec2 & vel)
{
	velocity.x = vel.x;
	velocity.y = vel.y;
}

void Entity::setPosition(const glm::ivec2 & pos)
{
	position = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

void Entity::setHorizontalVelocity(float vel)
{
	velocity.x = vel;
}

void Entity::setVerticalVelocity(float vel)
{
	velocity.y = vel;
}

void Entity::setAcceleration(const glm::vec2 & acc)
{
	acceleration.x = acc.x;
	acceleration.y = acc.y;
}

void Entity::setEdgePointer(int * edge) {
	mapEdge = edge;
}

void Entity::die() {
	return;
}

glm::ivec2 Entity::getHitBox() const
{
	return hitBox;
}

glm::ivec2 Entity::getPosition() const
{
	return position;
}

glm::vec2 Entity::getVelocity() const
{
	return velocity;
}

glm::vec2 Entity::getAcceleration() const
{
	return glm::ivec2();
}

bool Entity::isDead() const
{
	return death;
}

bool Entity::isDying() const
{
	return dying;
}
