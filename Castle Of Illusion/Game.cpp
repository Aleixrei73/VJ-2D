#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"


void Game::init()
{
	escena = 5;
	pantalla = 0;
	bPlay = true;
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	inicio.init();
	scene.init();
	instructions.init();
	creditos.init();

}

bool Game::update(int deltaTime)
{


	if (escena == 5) inicio.update(pantalla);
	else if (escena == 0) scene.update(deltaTime);
	else if (escena == 1) instructions.update();
	else if (escena == 2) creditos.update();
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (escena == 5) inicio.render();
	else if (escena == 0) scene.render();
	else if (escena == 1) instructions.render();
	else if (escena == 2) creditos.render();

}

void Game::keyPressed(int key)
{

	if (key == GLFW_KEY_SPACE) {
		if (escena == 1 || escena == 2) pantalla = 5;
		escena = pantalla;
		pantalla = 0;
	}
	if (key == GLFW_KEY_ESCAPE || escena == 3) // Escape code
		bPlay = false;
	keys[key] = true;
	if (escena == 5) {
		if (key == GLFW_KEY_W) {
			if (pantalla > 0) pantalla--;
		}
		if (key == GLFW_KEY_S) {
			if (pantalla < 3) pantalla++;
		}
	}



}

void Game::keyReleased(int key) {
	keys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

void Game::setScene(int scene) {
	escena = scene;
}
