#pragma once
#include <vector>
#include <algorithm>
#include "gameobject.h"
#include "tile.h"

class Pathfinder
{
public:
	Pathfinder();
	~Pathfinder();

	void SetRows(unsigned int _rows);
	void SetColumns(unsigned int _columns);
	void SetTileGrid(int row, int col, Tile* tile);
	void SetVecTiles(std::vector<GameObject*>* _vecTiles);
	void SetStartTile(Tile* start);
	void SetDestTile(Tile* dest);

	Tile* GetStartTile();
	Tile* GetDestTile();
	bool GetIsFinding() const;
	bool GetIsResizing() const;
	int GetRows() const;
	int GetColumns() const;

	void Starfinder();
	void Reset(bool resetObstacles);
	void ResizeGrid(int newRows, int newCols);
	int HeuristicCost(Tile* first, Tile* second); //Manhattan method of estimating heuristic value
	int FindDistance(Tile* current, Tile* neighbour); //Find distance based on angle between currrent and neighbour

private:
	unsigned int rows, columns;
	bool isFinding;
	bool isResizing;

	Tile* tileGrid[GRID_MAX_ROWS][GRID_MAX_COLS];

	Tile* startTile;
	Tile* destTile;
	Tile* currentTile;
	std::vector<GameObject*>* vecTiles;
	std::vector<Tile*> openList;		//List of nodes that have been visited but not expanded (neighbours not explored), pending tasks
	std::vector<Tile*> closedList;		//List of nodes that have been visited AND expanded (neighbours were explored and included in open list)
};