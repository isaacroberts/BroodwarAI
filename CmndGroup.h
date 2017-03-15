#pragma once

#include <vector>
#include <BWAPI.h>
#include "Util.h"
#include "Net.h"

using namespace BWAPI;

class CmndGroup
{
public:
	CmndGroup(void);
	~CmndGroup(void);
	void addUnit(Unit*);
	void calcMode();
	void move(Position pos)
	{	move(pos.x(),pos.y()); }
	void move(int x,int y);
	CmndGroup* getID() {
		return this;   }
private:
	int ID;
	std::vector<Unit*> unit;
	UnitType* modeType;
	Net *net;
};
