#pragma once

#include "Entity.h"

enum State {PICKED, FREE, THROWED};

class Barrel : public Entity {

public:

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, bool explosive);
	void update(int deltaTime);
	void render();
	void setState(State st);
	State getState();

private:
	void explode();
	bool explosive;
	bool exploded;
	State state;

};
