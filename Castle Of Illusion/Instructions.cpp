﻿#include "Instructions.h"
#include "Game.h"
#include <iostream>

#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 0
#define INIT_PLAYER_Y_TILES 6


void Instructions::init()
{
	initShaders();

	tileGap = 20;


	glm::vec2 basePosition;

	basePosition.x = basePosition.x = SCREEN_WIDTH / 16;;
	basePosition.y = SCREEN_HEIGHT / 16;

	font.loadFromFile("images/Font.png", TEXTURE_PIXEL_FORMAT_RGBA);

	float shiftCenter = ((TILE_SIZE - tileGap) * 6) / 2.f;


	glm::vec2 livesPos = basePosition - glm::vec2(shiftCenter, 0);

	int Space = 0;

	livesSprites.push_back(createLetter('W', livesPos, texProgram));
	livesSprites.push_back(createLetter('�', livesPos + glm::vec2(LETTER_GAP, 0), texProgram));
	livesSprites.push_back(createLetter('S', livesPos + glm::vec2(LETTER_GAP * 2, 0), texProgram));
	livesSprites.push_back(createLetter('A', livesPos + glm::vec2(LETTER_GAP * 3, 0), texProgram));
	livesSprites.push_back(createLetter('L', livesPos + glm::vec2(LETTER_GAP * 4, 0), texProgram));
	livesSprites.push_back(createLetter('T', livesPos + glm::vec2(LETTER_GAP * 5, 0), texProgram));
	livesSprites.push_back(createLetter('A', livesPos + glm::vec2(LETTER_GAP * 6, 0), texProgram));
	livesSprites.push_back(createLetter('R', livesPos + glm::vec2(LETTER_GAP * 7, 0), texProgram));

	Space += 3;

	livesSprites.push_back(createLetter('S', livesPos + glm::vec2(0, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('�', livesPos + glm::vec2(LETTER_GAP, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('C', livesPos + glm::vec2(LETTER_GAP * 2, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('A', livesPos + glm::vec2(LETTER_GAP * 3, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('U', livesPos + glm::vec2(LETTER_GAP * 4, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('R', livesPos + glm::vec2(LETTER_GAP * 5, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('E', livesPos + glm::vec2(LETTER_GAP * 6, LETTER_GAP * Space), texProgram));

	Space += 3;

	livesSprites.push_back(createLetter('D', livesPos + glm::vec2(0, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('�', livesPos + glm::vec2(LETTER_GAP, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('D', livesPos + glm::vec2(LETTER_GAP * 2, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('R', livesPos + glm::vec2(LETTER_GAP * 3, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('E', livesPos + glm::vec2(LETTER_GAP * 4, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('T', livesPos + glm::vec2(LETTER_GAP * 5, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('A', livesPos + glm::vec2(LETTER_GAP * 6, LETTER_GAP * Space), texProgram));

	Space += 3;

	livesSprites.push_back(createLetter('A', livesPos + glm::vec2(0, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('�', livesPos + glm::vec2(LETTER_GAP, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('E', livesPos + glm::vec2(LETTER_GAP * 2, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('S', livesPos + glm::vec2(LETTER_GAP * 3, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('Q', livesPos + glm::vec2(LETTER_GAP * 4, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('U', livesPos + glm::vec2(LETTER_GAP * 5, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('E', livesPos + glm::vec2(LETTER_GAP * 6, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('R', livesPos + glm::vec2(LETTER_GAP * 7, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('R', livesPos + glm::vec2(LETTER_GAP * 8, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('A', livesPos + glm::vec2(LETTER_GAP * 9, LETTER_GAP * Space), texProgram));

	Space += 3;

	livesSprites.push_back(createLetter('V', livesPos + glm::vec2(0, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('�', livesPos + glm::vec2(LETTER_GAP, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('A', livesPos + glm::vec2(LETTER_GAP * 2, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('G', livesPos + glm::vec2(LETTER_GAP * 3, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('A', livesPos + glm::vec2(LETTER_GAP * 4, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('F', livesPos + glm::vec2(LETTER_GAP * 5, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('A', livesPos + glm::vec2(LETTER_GAP * 6, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('R', livesPos + glm::vec2(LETTER_GAP * 7, LETTER_GAP * Space), texProgram));

	Space += 5;
	livesPos = livesPos + glm::vec2(-25, 0);
	livesSprites.push_back(createLetter('S', livesPos + glm::vec2(0, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('P', livesPos + glm::vec2(LETTER_GAP, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('A', livesPos + glm::vec2(LETTER_GAP * 2, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('C', livesPos + glm::vec2(LETTER_GAP * 3, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('E', livesPos + glm::vec2(LETTER_GAP * 4, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('T', livesPos + glm::vec2(LETTER_GAP * 6, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('O', livesPos + glm::vec2(LETTER_GAP * 7, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('G', livesPos + glm::vec2(LETTER_GAP * 9, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('O', livesPos + glm::vec2(LETTER_GAP * 10, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('B', livesPos + glm::vec2(LETTER_GAP * 12, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('A', livesPos + glm::vec2(LETTER_GAP * 13, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('C', livesPos + glm::vec2(LETTER_GAP * 14, LETTER_GAP * Space), texProgram));
	livesSprites.push_back(createLetter('K', livesPos + glm::vec2(LETTER_GAP * 15, LETTER_GAP * Space), texProgram));

	projection = glm::ortho(0.f, float(SCREEN_WIDTH) / 8, 2 * float(SCREEN_HEIGHT) / 8, 0.f);

	currentTime = 0.0f;
}


void Instructions::update() {


}

void Instructions::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	for (Sprite* letter : livesSprites) {
		letter->render();
	}
}

void Instructions::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}


Sprite* Instructions::createLetter(char letter, const glm::vec2& pos, ShaderProgram& shaderProgram) {
	int letterNum = letter - 'A' + 15;
	int j = letterNum / 18 + 1;
	int i = letterNum % 18;
	Sprite* letterSprite = Sprite::createSprite(glm::ivec2(7, 9), glm::vec2(1 / 18.f, 1 / 6.f), &font, &shaderProgram);
	letterSprite->setNumberAnimations(1);

	letterSprite->setAnimationSpeed(0, 8);
	letterSprite->addKeyframe(0, glm::vec2(1 / 18.f * i, 1 / 6.f * j));

	letterSprite->changeAnimation(0);
	letterSprite->setPosition(glm::vec2(float(pos.x), float(pos.y)));
	return letterSprite;
}