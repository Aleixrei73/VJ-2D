#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Sprite.h"

#define TILE_SIZE 32
#define HEIGHT 3
#define LETTER_GAP 7
#define HEART_GAP 16

class GUI {

public:

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, int amp);
	void render();
	void update(const glm::vec2 &newPos);

private:

	int lives;
	int score;
	int timeLeft;
	int tries;
	Sprite* background;
	vector<Sprite*> livesSprites, triesSprites, scoreSprites, timeSprites;
	Texture backgroundColor, font, heart;
	int amplitude;
	float tileGap;

	Sprite* createLetter(char letter, const glm::vec2 &pos, ShaderProgram & shaderProgram);
	void createWord(string word, vector<Sprite*> &res, const glm::vec2 &pos, ShaderProgram & shaderProgram);
	void createHearts(const glm::vec2 &pos, ShaderProgram & shaderProgram);
	void createNumber(int num, int digitNumber, vector<Sprite*> &res, const glm::vec2 &pos, ShaderProgram & shaderProgram);
};