#include "InfMap.h"
#include <set>
using namespace BWAPI;

/*
higher values are a better place to be
as of now, only enemy and own units are calculated
*/

InfMap::InfMap()
{
	grid=std::vector<std::vector<int>>();
	for (int x=0;x<Broodwar->mapWidth();x++)
	{
		grid.push_back(std::vector<int>(Broodwar->mapWidth()));
		for (int y=0;y<Broodwar->mapWidth();y++)
		{
			grid[x][y]=0;
		}
	}
}
void InfMap::update()
{
	for (int n=0;n<Broodwar->mapWidth();n++)
	{
		for (int m=0;m<Broodwar->mapHeight();m++)
		{
			grid[n][m]=0;
			std::set<Unit*> whosHere;
			whosHere=Broodwar->getUnitsOnTile(n,m);
			for (std::set<Unit*>::iterator it;it!=whosHere.end();it++)
			{
				if (Ref::isDangerous(&(*it)->getType()))
				{
					if ((*it)->getPlayer()->isEnemy(Broodwar->self()))
						grid[n][m]-=(int)Ref::getDangerValue(&(*it)->getType())*((double)(*it)->getHitPoints()/(double)(*it)->getType().maxHitPoints()+.5);
					
					else if ((*it)->getPlayer()->getID()==Broodwar->self()->getID())
						grid[n][m]+=(int)Ref::getDangerValue(&(*it)->getType())*((double)(*it)->getHitPoints()/(double)(*it)->getType().maxHitPoints()+.5);	
				}
				else
				{
					if ((*it)->getPlayer()->isEnemy(Broodwar->self()))
						grid[n][m]+=5;
					//else if ((*it)->getPlayer()->getID()==Broodwar->self()->getID())
						
				}
			}
		}
	}
}
std::vector<std::vector<int>> InfMap::getArea(TilePosition center)
{
	std::vector<std::vector<int>> area=std::vector<std::vector<int>>(3);

}
InfMap::~InfMap()
{
}

//