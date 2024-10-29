#include "GUI.h"

enum HeartState {FILL, EMPTY};

void GUI::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram, int amp) {
	

	//Variable initialitation

	lives = 3;
	score = 0;
	timeLeft = 200;
	tries = 3;
	amplitude = amp;
	tileGap = amplitude / 4.f;
	timeTimer = 0;

	//Texture initialitation

	backgroundColor.loadFromFile("images/Black.png", TEXTURE_PIXEL_FORMAT_RGB);
	font.loadFromFile("images/Font.png", TEXTURE_PIXEL_FORMAT_RGBA);
	heart.loadFromFile("images/Hearts.png", TEXTURE_PIXEL_FORMAT_RGBA);

	//Background initialitation

	background = Sprite::createSprite(glm::ivec2(amplitude*TILE_SIZE, HEIGHT*TILE_SIZE), glm::vec2(1, 1), &backgroundColor, &shaderProgram);
	
	background->setNumberAnimations(0);
	background->changeAnimation(0);

	glm::vec2 basePosition;

	basePosition.x = tileMapPos.x;
	basePosition.y = tileMapPos.y;

	background->setPosition(glm::vec2(float(basePosition.x), float(basePosition.y)));

	//Names initialitation

	float shiftLetters = (tileGap*TILE_SIZE - 7 * 5)/2.f;
	float shiftSprite = (tileGap*TILE_SIZE - 3 * 16) / 2.f;

	glm::vec2 livesPos = basePosition + glm::vec2(shiftLetters,-3*TILE_SIZE);
	glm::vec2 heartsPos = basePosition + glm::vec2(shiftSprite, -2 * TILE_SIZE);

	shiftSprite = (tileGap*TILE_SIZE - 1 * 5) / 2.f;

	glm::vec2 triesPos = livesPos + glm::vec2(tileGap* TILE_SIZE, 0);
	glm::vec2 triesNumPos = triesPos + glm::vec2(-shiftLetters+shiftSprite, TILE_SIZE);

	shiftSprite = (tileGap*TILE_SIZE -  6 * 5) / 2.f;
	glm::vec2 scorePos = triesPos + glm::vec2(tileGap * TILE_SIZE, 0);
	glm::vec2 scoreNumPos = scorePos + glm::vec2(-LETTER_GAP/2.f, TILE_SIZE);

	shiftSprite = (tileGap*TILE_SIZE - 3 * 5) / 2.f;
	glm::vec2 timePos = scorePos + glm::vec2(tileGap * TILE_SIZE, 0);
	glm::vec2 timeNumPos = timePos + glm::vec2(-shiftLetters + shiftSprite, TILE_SIZE);

	createWord("POWER", livesSprites, livesPos, shaderProgram);
	createWord("TRIES", triesSprites, triesPos, shaderProgram);
	createWord("SCORE", scoreSprites, scorePos, shaderProgram);
	createWord(" TIME" , timeSprites, timePos, shaderProgram);

	//Resources initialitation

	createHearts(heartsPos, shaderProgram);
	createNumber(tries, 1, triesSprites, triesNumPos, shaderProgram);
	createNumber(score, 6, scoreSprites, scoreNumPos, shaderProgram);
	createNumber(timeLeft, 3, timeSprites, timeNumPos, shaderProgram);



}

void GUI::createNumber(int num, int digitNumber, vector<Sprite*>& res, const glm::vec2 & pos, ShaderProgram & shaderProgram) {

	string numStr = to_string(num);
	int n = numStr.length();
	int numBegin = 16;

	for (int i = digitNumber-1; i > -1; i--) {
		int num = 0;

		if (i < n) {
			num = numStr[n - i - 1] - '0';
		}

		Sprite *numSprite = Sprite::createSprite(glm::ivec2(7, 9), glm::vec2(1 / 18.f, 1 / 6.f), &font, &shaderProgram);
		numSprite->setNumberAnimations(10);

		for (int j = 0; j < 10; j++) {
			numSprite->setAnimationSpeed(j, 8);
			numSprite->addKeyframe(j, glm::vec2(1/18.f * ((j + 16)%18) , 1 / 6.f * ((j+16)/18)));
		}

		numSprite->changeAnimation(num);
		numSprite->setPosition(glm::vec2(pos.x+LETTER_GAP*((digitNumber - 1) - i), pos.y));

		res.push_back(numSprite);
	}
}

void GUI::createHearts(const glm::vec2 &pos, ShaderProgram & shaderProgram) {

	for (int i = 0; i < 3; i++) {
		Sprite *heartSprite = Sprite::createSprite(glm::ivec2(16,16), glm::vec2(1/2.f, 1), &heart, &shaderProgram);
		heartSprite->setNumberAnimations(2);
		heartSprite->setAnimationSpeed(FILL, 1);
		heartSprite->addKeyframe(FILL, glm::vec2(0.f, 0.f));
		heartSprite->setAnimationSpeed(EMPTY, 1);
		heartSprite->addKeyframe(EMPTY, glm::vec2(0.5f, 0.f));
		heartSprite->changeAnimation(0);
		heartSprite->setPosition(pos + glm::vec2(HEART_GAP*i,0));
		livesSprites.push_back(heartSprite);
	}

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

void GUI::update(const glm::vec2 & newPos, int deltaTime) {

	background->setPosition(newPos);

	float shiftLetters = (tileGap*TILE_SIZE - 7*5) /2.f;

	glm::vec2 livesPos = newPos + glm::vec2(shiftLetters, -3 * TILE_SIZE + 9);
	float shiftSprite = (tileGap*TILE_SIZE - 3 * 16) / 2.f;
	glm::vec2 heartsPos = newPos + glm::vec2(shiftSprite, -2 * TILE_SIZE);

	glm::vec2 triesPos = livesPos + glm::vec2(tileGap * TILE_SIZE, 0);
	shiftSprite = (tileGap*TILE_SIZE - 1 * 5) / 2.f;
	glm::vec2 triesNumPos = glm::vec2(-shiftLetters + shiftSprite + triesPos.x, -2*TILE_SIZE + newPos.y - 9 / 2.f + 1);

	glm::vec2 scorePos = triesPos + glm::vec2(tileGap* TILE_SIZE, 0);
	glm::vec2 scoreNumPos = glm::vec2(-LETTER_GAP / 2.f + scorePos.x, -2 * TILE_SIZE + newPos.y - 9 / 2.f + 1);

	shiftSprite = (tileGap*TILE_SIZE - 3 * 5) / 2.f;
	glm::vec2 timePos = scorePos + glm::vec2(tileGap * TILE_SIZE, 0);
	glm::vec2 timeNumPos = glm::vec2(-shiftLetters + shiftSprite + timePos.x, -2 * TILE_SIZE + newPos.y - 9 / 2.f + 1);

	timeTimer += deltaTime;

	while (timeTimer >= 1000) {
		timeTimer -= 1000;
		timeLeft -= 1;
	}


	//Lives update

	for (int i = 0; i < 5; i++) {
		livesSprites[i]->setPosition(livesPos + glm::vec2(LETTER_GAP*i, 0));
	}

	for (int i = 5; i < 8; i++) {
		if ((i - 5) < lives) {
			livesSprites[i]->changeAnimation(FILL);
		}
		else livesSprites[i]->changeAnimation(EMPTY);
		livesSprites[i]->setPosition(heartsPos + glm::vec2(HEART_GAP*(i-5), 0));
	}

	//Tries update

	for (int i = 0; i < 5; i++) {
		triesSprites[i]->setPosition(triesPos + glm::vec2(LETTER_GAP*i, 0));
	}

	for (int i = 5; i < 6; i++) {
		triesSprites[i]->setPosition(triesNumPos + glm::vec2(LETTER_GAP*(i - 5), 0));
		triesSprites[i]->changeAnimation(tries);
	}

	//Score update

	for (int i = 0; i < 5; i++) {
		scoreSprites[i]->setPosition(scorePos + glm::vec2(LETTER_GAP*i, 0));
	}

	for (int i = 10; i > 4; i--) {
		scoreSprites[i]->setPosition(scoreNumPos + glm::vec2(LETTER_GAP*(i - 5), 0));
		scoreSprites[i]->changeAnimation((score/int(pow(10, (10 - i))))%10);
	}


	//Time update

	for (int i = 0; i < 5; i++) {
		timeSprites[i]->setPosition(timePos + glm::vec2(LETTER_GAP*i, 0));
	}

	for (int i = 7; i > 4; i--) {
		timeSprites[i]->setPosition(timeNumPos + glm::vec2(LETTER_GAP*(i - 5), 0));
		timeSprites[i]->changeAnimation((timeLeft / int(pow(10, (7 - i)))) % 10);
	}


}

int GUI::getLives() {
	return lives;
}

int GUI::getScore() {
	return score;
}

int GUI::getTries() {
	return tries;
}

int GUI::getTimeLeft() {
	return timeLeft;
}

void GUI::setLives(int liv) {
	lives = liv;
}

void GUI::setTries(int tri) {
	tries = tri;
}

void GUI::setScore(int scr) {
	score = scr;
}

void GUI::setTimeLeft(int time) {
	timeLeft = time;
}

void GUI::createWord(string word, vector<Sprite*>& res, const glm::vec2 &pos, ShaderProgram & shaderProgram) {

	int n = word.length();

	for (int i = 0; i < n; i++) {
		res.push_back(createLetter(word[i], pos + glm::vec2(LETTER_GAP*i, 0), shaderProgram));
	}
}

Sprite * GUI::createLetter(char letter, const glm::vec2 & pos, ShaderProgram & shaderProgram) {
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
