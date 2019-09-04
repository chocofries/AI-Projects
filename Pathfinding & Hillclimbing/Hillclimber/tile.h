#pragma once
#include "gameobject.h"

class Hillclimber;

enum TileState
{
	TILE_FREE,	//The tile is free
	TILE_START,	//The start search tile
	TILE_FOUND,	//The found tile
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
	float GenerateValue();

	TileState GetTileState() const;
	int GetGridX() const;
	int GetGridY() const;
	float GetValue() const;
	Tile* GetParentTile();
	

	void SetHillclimber(Hillclimber* p);
	void SetTileState(TileState _state);
	void SetParentTile(Tile* pt);
	void SetGridX(int _x);
	void SetGridY(int _y);
	void SetValue(float _v);
	void SetTextActive(bool isText);

private:
	TileState tileState;

	float tileValue; //From 0 to 1

	bool isActive;

	int gridX, gridY;

	Texter* text;

	Hillclimber* hillclimber;
	Tile* parentTile;
};