#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include <GLFW/glfw3.h>
#include "Scene.h"
#include "Inicio.h"
#include "Creditos.h"
#include "Instructions.h"
#include "GUI.h"
#include "SoundManager.h"

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900


// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

private:
	Game() {}

public:
	static Game& instance()
	{
		static Game G;

		return G;
	}

	void init();
	bool update(int deltaTime);
	void render();

	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);

	bool getKey(int key) const;
	void setScene(int scene);
	void nextLevel();
	void startAgain();

private:
	bool bPlay; // Continue to play game?
	bool keys[GLFW_KEY_LAST + 1]; // Store key states so that 
								  // we can have access at any time
	int pantalla;
	int escena;
	int level;

	Scene level01, level02, level03;
	Inicio inicio;
	Creditos creditos;
	Instructions instructions;

};


#endif // _GAME_INCLUDE

