#pragma once

#include "Entity.h"

enum State {PICKED, FREE, THROWED};

class Barrel : public Entity {

public:

	~Barrel();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, bool explosive);
	void update(int deltaTime);
	void render();
	void setState(State st);
	State getState();
	bool isExplosive() const;
	void explode();
	bool hasExploded() const;

private:
	
	bool explosive;
	bool exploded;
	State state;
	int explodeTime;

};
