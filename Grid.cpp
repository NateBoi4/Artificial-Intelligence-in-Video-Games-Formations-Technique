#include "Grid.h"
#include "Game.h"
#include "GraphicsSystem.h"
#include "System.h"
#include "Vector2D.h"

Grid::Grid()
{
	mWidth = (int)gpGame->getGraphicsSystem()->getDisplayWidth() / 32;
	mHeight = (int)gpGame->getGraphicsSystem()->getDisplayHeight() / 32;
}

Grid::~Grid()
{
}

void Grid::initGrid()
{
	for (int i = 0; i < mWidth; i++) 
	{
		for (int j = 0; j < mHeight; j++) 
		{
			mCells[mCellIndex] = Vector2D(((int)gpGame->getGraphicsSystem()->getDisplayWidth() / 32) * i, ((int)gpGame->getGraphicsSystem()->getDisplayHeight() / 32) * j);
			mCellIndex++;
		}
	}
}
