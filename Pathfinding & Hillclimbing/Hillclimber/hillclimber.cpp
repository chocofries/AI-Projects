#include "hillclimber.h"

Hillclimber::Hillclimber()
{
	isFinding = false;
	isResizing = false;
	searchRadius = 3; //Number of tiles from the center of the start tile to search through each loop
	searchIterations = 10; //How many times to iterate the loop
}

Hillclimber::~Hillclimber()
{
}

void Hillclimber::SetRows(unsigned int _rows)
{
	rows = _rows;
}

void Hillclimber::SetColumns(unsigned int _columns)
{
	columns = _columns;
}

void Hillclimber::SetTileGrid(int row, int col, Tile * tile)
{
	tileGrid[row][col] = tile;
}

void Hillclimber::SetVecTiles(std::vector<GameObject*>* _vecTiles)
{
	vecTiles = _vecTiles;
}

void Hillclimber::SetStartTile(Tile * start)
{
	startTile = start;
}

void Hillclimber::SetSearchRadius(int _r)
{
	searchRadius = _r;
}

void Hillclimber::SetSearchIterations(int _i)
{
	searchIterations = _i;
}

Tile * Hillclimber::GetStartTile()
{
	return startTile;
}

bool Hillclimber::GetIsFinding() const
{
	return isFinding;
}

bool Hillclimber::GetIsResizing() const
{
	return isResizing;
}

int Hillclimber::GetRows() const
{
	return rows;
}

int Hillclimber::GetColumns() const
{
	return columns;
}

int Hillclimber::GetSearchRadius() const
{
	return searchRadius;
}

int Hillclimber::GetSearchIterations() const
{
	return searchIterations;
}

void Hillclimber::Starfinder()
{
	Reset(false);
	isFinding = true;

	int tX = startTile->GetGridX();
	int tY = startTile->GetGridY();

	//Search all of the tiles in the starting tile's region until no larger value can be found
	float bestValue = startTile->GetValue();
	Tile* bestTile = startTile;

	for (int it = searchIterations; it > 0; it--)
	{
		for (int i = -searchRadius; i <= searchRadius; i++)
		{
			for (int j = -searchRadius; j <= searchRadius; j++)
			{
				int iX = tX + i;
				int iY = tY + j;

				if (iX >= 0 && iY >= 0 && iX < rows && iY < columns) //If within bounds of the grid
				{
					Tile* temp = tileGrid[iX][iY];
					if (temp->GetValue() > bestValue)
					{
						bestValue = temp->GetValue();
						bestTile = temp;
					}
				}
			}
		}

		//Change to search from the highest-value tile
		tX = bestTile->GetGridX();
		tY = bestTile->GetGridY();
	}

	bestTile->SetTileState(TILE_FOUND);
	isFinding = false;
}

void Hillclimber::Reset(bool resetObstacles)
{
	for (int i = 0; i < GRID_MAX_ROWS; i++)
	{
		for (int j = 0; j < GRID_MAX_COLS; j++)
		{
			Tile* temp = tileGrid[i][j];

			if (temp != nullptr)
			{
				TileState tileState = temp->GetTileState();

				if (tileState != TILE_START)
				{
					temp->SetTileState(TILE_FREE);
				}
			}
		}
	}
}

void Hillclimber::ResizeGrid(int newRows, int newCols)
{
	isResizing = true;
	if (!isFinding)
	{
		if (newRows <= GRID_MAX_ROWS && newCols <= GRID_MAX_COLS)
		{
			int prevRows = rows;
			int prevCols = columns;
			rows = newRows;
			columns = newCols;

			float halfRow = (float)(rows - 1) * 0.5f;
			float halfColumn = (float)(columns - 1) * 0.5f;
			float tileScale = (float)((WINDOW_WIDTH + 100) / columns);
			float gap = 1.0f;
			float offset = (tileScale*0.5f) + gap;

			startTile->SetTileState(TILE_FREE);

			for (int i = 0; i < GRID_MAX_ROWS; i++)
			{
				for (int j = 0; j < GRID_MAX_COLS; j++)
				{
					Tile* temp = tileGrid[i][j];

					if (temp != nullptr)
					{
						if (i >= newRows || j >= newCols) //Out of bounds
						{
							temp->SetX((offset*(float)i) - (halfColumn * offset));
							temp->SetY((offset*-(float)j) + (halfRow * offset));
							temp->SetXScale(tileScale);
							temp->SetYScale(tileScale);
							temp->SetTextActive(false);
							temp->SetIsActive(false);
						}
						else //Within bounds
						{
							if (i == 0 && j == 0)
							{
								temp->SetTileState(TILE_START);
								startTile = temp;
							}
							temp->SetX((offset*(float)i) - (halfColumn * offset));
							temp->SetY((offset*-(float)j) + (halfRow * offset));
							temp->SetXScale(tileScale);
							temp->SetYScale(tileScale);
							temp->SetIsActive(true);
						}
					}
				}
			}
		}
	}
	isResizing = false;
}