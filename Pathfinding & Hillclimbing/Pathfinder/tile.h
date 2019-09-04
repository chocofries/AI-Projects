#pragma once
#include "gameobject.h"

class Pathfinder;

enum TileState
{
	TILE_FREE,	//The tile is free
	TILE_START,	//The starting tile of pathfinding
	TILE_DEST,	//The destination tile of pathfinding
	TILE_OBS,	//The tile is an obstacle
	TILE_OPEN,	//The tile is being calculated
	TILE_CLOSED,  //The tile has been calculated
	TILE_PATH	//The tile is part of the final path
};

class Tile : public GameObject
{
public:
	Tile();
	virtual ~Tile();

	virtual void Initialise();
	virtual void Update();
	virtual void Draw();

	void ProcessInput();

	TileState GetTileState() const;
	int GetScoreF() const;
	int GetScoreG() const;
	int GetScoreH() const;
	int GetGridX() const;
	int GetGridY() const;
	Tile* GetParentTile();
	
	void SetPathfinder(Pathfinder* p);
	void SetTileState(TileState _state);
	void SetParentTile(Tile* pt);
	void SetRayOrigin(glm::vec3* rO);
	void SetRayWorld(glm::vec3* rW);
	void SetScoreF(int _F);
	void SetScoreG(int _G);
	void SetScoreH(int _H);
	void SetGridX(int _x);
	void SetGridY(int _y);
	void SetTextActive(bool isText);

private:
	glm::vec3* rayOrigin;
	glm::vec3* rayWorld;

	TileState tileState;

	bool isActive;

	int gridX, gridY;
	int scoreF, scoreG, scoreH;

	Texter* textF;
	Texter* textG;
	Texter* textH;

	Pathfinder* pathfinder;
	Tile* parentTile;
};