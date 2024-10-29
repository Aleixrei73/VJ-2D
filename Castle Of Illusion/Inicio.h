#ifndef _INICI_INCLUDE
#define _INICI_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Sprite.h"


class Inicio
{

public:
	void init();
	void update(int pantalla);
	void render();

private:
	void initShaders();
	Texture font;
	Texture fletxa;
	Sprite* heartSprite;
	ShaderProgram texProgram;
	float currentTime;
	glm::ivec2 cameraPos;
	glm::mat4 projection;
	bool interacting;
	float tileGap;
	int amplitude;
	vector<Sprite*> livesSprites, triesSprites, scoreSprites, timeSprites;
	Sprite* createLetter(char letter, const glm::vec2& pos, ShaderProgram& shaderProgram);
};

#endif // _INICI_INCLUDE