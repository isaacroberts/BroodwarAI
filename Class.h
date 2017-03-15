#pragma once

#include "BWAPI.h"
#include "NetHandler.h"

using namespace BWAPI;

class Class
{
public:
	Class();
	Class(const UnitType);
	void setPermInputs();
	void updateInputs();

	UnitType getType() {return type;}
	double getDangerVal() {return dangerVal;}
	double getKillVal() {return killVal;}
	~Class();

	bool isSuicidal() {return isSuicidal(&type);}
	bool isTrim() {return isTrim(&type);}

	static double abilitySpecialDangerVal(TechType);
	static bool isSuicidal(UnitType*);
	static double gasToMinValRatio;
	static bool isTrim(UnitType*);
private:
	UnitType  type;
	Net* net;
	double dangerVal,killVal;//add coefficients to these
};
