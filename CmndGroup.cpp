#include "CmndGroup.h"

using namespace BWAPI;

CmndGroup::CmndGroup()
{
	unit=std::vector<Unit*>();
	//set up neur[] here
}
void CmndGroup::addUnit(Unit *add)
{
	unit.push_back(add);
}
void CmndGroup::move(int mx,int my)
{
	int wid=Util::roughSquareRoot((int)unit.size());
	double heit=(double)unit.size()/(double)wid;
	int xOffset=0;
	for (int n=0;n<wid;n++)
		xOffset+=(unit[n])->getType().dimensionLeft();
	int x=mx-xOffset;
	int y=my;
	for (int n=0;n<heit;n++)
		y-=(unit[n])->getType().dimensionUp();
	unsigned int ix=0;
	for (int n=0;n<heit;n++)
	{
		if (ix>unit.size())
			break;
		int talst=0;
		for (int m=0;m<wid;m++)
		{
			if (ix>unit.size())
				break;
			Position pos=Position(x,y);
			(unit[ix])->move(pos);
			x+=(unit[ix])->getType().dimensionLeft();
			x+=(unit[ix])->getType().dimensionRight();
			if ((unit[ix])->getType().dimensionUp()>talst)
				talst=(unit[ix])->getType().dimensionUp();
			ix++;
		}
		y+=talst*2;
	}
}
void CmndGroup::calcMode()
{
	std::vector<int> amt;
	std::vector<UnitType*> type;

	for (unsigned int n=0;n<unit.size();n++)
	{
		bool pBack=true;
		for (unsigned int m=0;m<type.size();m++)
		{
			if ((unit[n])->getType().getID()==type[m]->getID())
			{
				amt[m]++;
				m=type.size();
				pBack=false;
			}
		}
		if (pBack)
		{
			type.push_back(&(unit[n])->getType());
			amt.push_back(1);
		}
	}
}
CmndGroup::~CmndGroup()
{
	//delete neur[];
}
