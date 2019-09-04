#pragma once
#include <vector>
#include <algorithm>
#include "gameobject.h"
#include "tile.h"

class Hillclimber
{
public:
	Hillclimber();
	~Hillclimber();

	void SetRows(unsigned int _rows);
	void SetColumns(unsigned int _columns);
	void SetTileGrid(int row, int col, Tile* tile);
	void SetVecTiles(std::vector<GameObject*>* _vecTiles);
	void SetStartTile(Tile* start);
	void SetSearchRadius(int _r);
	void SetSearchIterations(int _i);

	Tile* GetStartTile();
	bool GetIsFinding() const;
	bool GetIsResizing() const;
	int GetRows() const;
	int GetColumns() const;
	int GetSearchRadius() const;
	int GetSearchIterations() const;

	void Starfinder(); //Hillclimbing algorithm to find the brightest star
	void Reset(bool resetObstacles);
	void ResizeGrid(int newRows, int newCols);

private:
	int searchRadius;
	int searchIterations;
	unsigned int rows, columns;
	bool isFinding;
	bool isResizing;

	Tile* tileGrid[GRID_MAX_ROWS][GRID_MAX_COLS];

	Tile* startTile;
	Tile* currentTile;
	std::vector<GameObject*>* vecTiles;
	std::vector<Tile*> openList;		//List of nodes that have been visited but not expanded (neighbours not explored), pending tasks
	std::vector<Tile*> closedList;		//List of nodes that have been visited AND expanded (neighbours were explored and included in open list)
};