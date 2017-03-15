#pragma once

#include "BWAPI.h"

using namespace BWAPI;

class BuildOrder
{
public:
	BuildOrder() {}
	BuildOrder(UnitType* type,Position at) {
		what=*type;
		place=at;
		urgency=3;
	}
	BuildOrder(const UnitType type,Position at)
	{	
		what=type;
		place=at;
		urgency=3;
	}
	BuildOrder(UnitType* type) {
		what=*type;
		place=Position(-1,-1);
		urgency=3;
	}
	BuildOrder (const UnitType type)
	{
		what=type;
		place=Position(-1,-1);
		urgency=3;
	}
	void setUrgency(int set) {
		urgency=set;
	}
	int getUrgency() {
		return urgency;
	}
	UnitType* getType() {
		return (&what);
	}
	bool hasPos() {
		return (place.x()!=-1 && place.y()!=-1);
	}
	Position getPos() {
		return place;
	}
	~BuildOrder(void) {}
private:
	UnitType what;
	Position place;
	int urgency;//between 1 and 5
};
