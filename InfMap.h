#pragma once

#include <vector>
#include <BWAPI.h>
#include "Util.h"
#include "Ref.h"

using namespace BWAPI;

class InfMap
{
public:
	InfMap();
	void update();
	int getVal(TilePosition pos) {
		return grid[pos.x()][pos.y()];	}
	int getVal(int x,int y) {
		return grid[x][y];	}
	std::vector<std::vector<int>> getArea(TilePosition);
	~InfMap();
private:
	std::vector<std::vector<int>> grid;
};
