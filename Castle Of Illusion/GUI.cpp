#include "GUI.h"

enum letters {A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, W, X, Y, Z};

void GUI::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram, int amp) {
	
	lives = 10;
	score = 0;
	timeLeft = 200;
	tries = 3;
	amplitude = amp;
	tileGap = amplitude / 6.f;
	backgroundColor.loadFromFile("images/Black.png", TEXTURE_PIXEL_FORMAT_RGB);
	background = Sprite::createSprite(glm::ivec2(amplitude*TILE_SIZE, HEIGHT*TILE_SIZE), glm::vec2(1, 1), &backgroundColor, &shaderProgram);
	
	background->setNumberAnimations(0);
	background->changeAnimation(0);

	glm::vec2 basePosition;

	basePosition.x = tileMapPos.x;
	basePosition.y = tileMapPos.y;

	background->setPosition(glm::vec2(float(basePosition.x), float(basePosition.y)));

	font.loadFromFile("images/Font.png", TEXTURE_PIXEL_FORMAT_RGBA);

	glm::vec2 livesPos = basePosition + glm::vec2(tileGap*TILE_SIZE,-2*TILE_SIZE);

	livesSprites.push_back(createLetter('P', livesPos, shaderProgram));
	livesSprites.push_back(createLetter('O', livesPos + glm::vec2(LETTER_GAP,0), shaderProgram));
	livesSprites.push_back(createLetter('W', livesPos + glm::vec2(LETTER_GAP*2, 0), shaderProgram));
	livesSprites.push_back(createLetter('E', livesPos + glm::vec2(LETTER_GAP * 3, 0), shaderProgram));
	livesSprites.push_back(createLetter('R', livesPos + glm::vec2(LETTER_GAP * 4, 0), shaderProgram));

	glm::vec2 triesPos = livesPos + glm::vec2(tileGap* TILE_SIZE, 0);

	triesSprites.push_back(createLetter('T', triesPos, shaderProgram));
	triesSprites.push_back(createLetter('R', triesPos + glm::vec2(LETTER_GAP, 0), shaderProgram));
	triesSprites.push_back(createLetter('I', triesPos + glm::vec2(LETTER_GAP * 2, 0), shaderProgram));
	triesSprites.push_back(createLetter('E', triesPos + glm::vec2(LETTER_GAP * 3, 0), shaderProgram));
	triesSprites.push_back(createLetter('S', triesPos + glm::vec2(LETTER_GAP * 4, 0), shaderProgram));

	glm::vec2 scorePos = triesPos + glm::vec2(tileGap * TILE_SIZE, 0);

	scoreSprites.push_back(createLetter('S', scorePos, shaderProgram));
	scoreSprites.push_back(createLetter('C', scorePos + glm::vec2(LETTER_GAP, 0), shaderProgram));
	scoreSprites.push_back(createLetter('O', scorePos + glm::vec2(LETTER_GAP * 2, 0), shaderProgram));
	scoreSprites.push_back(createLetter('R', scorePos + glm::vec2(LETTER_GAP * 3, 0), shaderProgram));
	scoreSprites.push_back(createLetter('E', scorePos + glm::vec2(LETTER_GAP * 4, 0), shaderProgram));

	glm::vec2 timePos = scorePos + glm::vec2(tileGap * TILE_SIZE, 0);

	timeSprites.push_back(createLetter('T', timePos, shaderProgram));
	timeSprites.push_back(createLetter('I', timePos + glm::vec2(LETTER_GAP, 0), shaderProgram));
	timeSprites.push_back(createLetter('M', timePos + glm::vec2(LETTER_GAP * 2, 0), shaderProgram));
	timeSprites.push_back(createLetter('E', timePos + glm::vec2(LETTER_GAP * 3, 0), shaderProgram));




}

void GUI::render() {
	background->render();
	for (Sprite* letter : livesSprites) {
		letter->render();
	}

	for (Sprite* letter : triesSprites) {
		letter->render();
	}

	for (Sprite* letter : scoreSprites) {
		letter->render();
	}

	for (Sprite* letter : timeSprites) {
		letter->render();
	}
}

void GUI::update(const glm::vec2 & newPos) {
	background->setPosition(newPos);

	int n = livesSprites.size();
	glm::vec2 livesPos = newPos + glm::vec2(tileGap * TILE_SIZE, -2 * TILE_SIZE);

	for (int i = 0; i < n; i++) {
		livesSprites[i]->setPosition(livesPos + glm::vec2(LETTER_GAP*i, 0));
	}

	n = triesSprites.size();
	glm::vec2 triesPos = livesPos + glm::vec2(tileGap * TILE_SIZE, 0);

	for (int i = 0; i < n; i++) {
		triesSprites[i]->setPosition(triesPos + glm::vec2(LETTER_GAP*i, 0));
	}

	n = scoreSprites.size();
	glm::vec2 scorePos = triesPos + glm::vec2(tileGap* TILE_SIZE, 0);

	for (int i = 0; i < n; i++) {
		scoreSprites[i]->setPosition(scorePos + glm::vec2(LETTER_GAP*i, 0));
	}

	n = timeSprites.size();
	glm::vec2 timePos = scorePos + glm::vec2(tileGap * TILE_SIZE, 0);

	for (int i = 0; i < n; i++) {
		timeSprites[i]->setPosition(timePos + glm::vec2(LETTER_GAP*i, 0));
	}


}

Sprite * GUI::createLetter(char letter, const glm::ivec2 & pos, ShaderProgram & shaderProgram) {
	int letterNum = letter - 'A' + 15;
	int j = letterNum / 18 + 1;
	int i = letterNum % 18;
	Sprite* letterSprite = Sprite::createSprite(glm::ivec2(7, 9), glm::vec2(1/18.f, 1/6.f), &font, &shaderProgram);
	letterSprite->setNumberAnimations(1);

	letterSprite->setAnimationSpeed(0, 8);
	letterSprite->addKeyframe(0, glm::vec2(1/18.f * i, 1/6.f * j));

	letterSprite->changeAnimation(0);
	letterSprite->setPosition(glm::vec2(float(pos.x), float(pos.y)));
	return letterSprite;
}
