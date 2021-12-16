#pragma once
#include "Vector2D.h"
#include <vector>

class Grid : public Trackable
{
public:
	Grid();
	~Grid();

	void initGrid();

	//Use enum?
	/*enum Slots
	{
		INVALID_SLOT = -1,
		OPEN_SLOT = 0,
		LEADER_SLOT = 1,
		UNIT_SLOT = 2
	};*/

private:
	int mWidth;
	int mHeight;

	std::vector<Vector2D> mCells;
	int mCellIndex = 0;
};

