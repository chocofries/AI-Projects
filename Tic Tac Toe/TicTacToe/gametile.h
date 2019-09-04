#pragma once

#include <iostream>
#include <time.h>
#include <SOIL.h>
#include "ShaderLoader.h"
#include "game.h"

class Game;

class GameTile
{

public:
	GameTile();
	~GameTile();
	
	void Initialise();
	void Draw();
	void Update();
	void CheckClick(int _x, int _y);

	float GetTileX() const;
	float GetTileY() const;
	void SetTileX(float _x);
	void SetTileY(float _y);
	void SetArrayX(int _x);
	void SetArrayY(int _y);
	void SetVBO(GLuint* _VBO);
	void SetGame(Game* _game);


private:
	GLuint* VBO;
	GLuint quadVAO;
	GLuint quadEBO;
	GLuint program;
	GLuint tex;
	GLuint tex2;
	GLuint tex3;

	Game* myGame;

	float tileX;
	float tileY;
	int realX;
	int realY;
	int arrayX;
	int arrayY;

	float scrollU;
	float scrollV;

	float isMarked;
	float circleCross; // 0 = circle, 1 = cross

};