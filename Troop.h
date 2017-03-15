#pragma once

#include "BWAPI.h"
#include "Area.h"

using namespace BWAPI;
class Troop
{
public:
	Troop();
	Troop(Unit*);
	void autonomous();
	Unit* getUnit() {return unit;}
	~Troop();
private:
	Unit* unit;
};
