#include <math.h>
#include "hillclimberSA.h"

HillclimberSA::HillclimberSA()
{
	isFinding = false;
	isResizing = false;
	searchRadius = 3; //Number of tiles from the center of the start tile to search through each loop
	searchTemp = 1000; //Initial temperature of the simulated annealing algorithm
}

HillclimberSA::~HillclimberSA()
{
}

void HillclimberSA::SetRows(unsigned int _rows)
{
	rows = _rows;
}

void HillclimberSA::SetColumns(unsigned int _columns)
{
	columns = _columns;
}

void HillclimberSA::SetTileGrid(int row, int col, Tile * tile)
{
	tileGrid[row][col] = tile;
}

void HillclimberSA::SetVecTiles(std::vector<GameObject*>* _vecTiles)
{
	vecTiles = _vecTiles;
}

void HillclimberSA::SetStartTile(Tile * start)
{
	startTile = start;
}

void HillclimberSA::SetSearchRadius(int _r)
{
	searchRadius = _r;
}

void HillclimberSA::SetSearchTemp(int _i)
{
	searchTemp = _i;
}

Tile * HillclimberSA::GetStartTile()
{
	return startTile;
}

bool HillclimberSA::GetIsFinding() const
{
	return isFinding;
}

bool HillclimberSA::GetIsResizing() const
{
	return isResizing;
}

int HillclimberSA::GetRows() const
{
	return rows;
}

int HillclimberSA::GetColumns() const
{
	return columns;
}

int HillclimberSA::GetSearchRadius() const
{
	return searchRadius;
}

int HillclimberSA::GetSearchTemp() const
{
	return searchTemp;
}

void HillclimberSA::StarfinderSA()
{
	Reset(false);
	startTile->SetTileState(TILE_START);
	isFinding = true;

	int beginT = searchTemp;
	int finalT = 1;
	int T = beginT;

	Tile* bestTile = startTile;
	float bestValue = bestTile->GetValue();

	int tX = bestTile->GetGridX();
	int tY = bestTile->GetGridY();
	int newX;
	int newY;

	int searches = 0;

	while (T > finalT)
	{
		//Perturb the position
		newX = tX + (rows - (rand() % (rows * 2)));
		newY = tY + (columns - (rand() % (columns * 2)));

		if (newX >= 0 && newY >= 0 && newX < (int)rows && newY < (int)columns) //If within bounds of the grid
		{	
			searches++;
			for (int i = -searchRadius; i <= searchRadius; i++)
			{
				for (int j = -searchRadius; j <= searchRadius; j++)
				{
					int iX = tX + i;
					int iY = tY + j;

					if (iX >= 0 && iY >= 0 && iX < (int)rows && iY < (int)columns) //If within bounds of the grid
					{
						Tile* tempTile = tileGrid[newX][newY];

						float deltaC = -(tempTile->GetValue()) - bestValue;
						float chance = exp(deltaC /T);

						
						if (T == 300)
						{
							int yup = 0;
						}

						if (deltaC < 0.0f)
						{
							bestTile = tempTile;
							bestValue = -(tempTile->GetValue());
						}
						else if (exp(deltaC/T) < rand()%2)
						{
							bestTile = tempTile;
							bestValue = -(tempTile->GetValue());
						}
					}
				}
			}

			tX = bestTile->GetGridX();
			tY = bestTile->GetGridY();
		}
		else
		{
			continue;
		}

		T -= 1;
	}

	bestTile->SetTileState(TILE_FOUND);
	isFinding = false;
}

void HillclimberSA::Reset(bool resetObstacles)
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

void HillclimberSA::ResizeGrid(int newRows, int newCols)
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