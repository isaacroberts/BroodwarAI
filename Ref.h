#pragma once

#include <set>
#include <vector>
#include "BWAPI.h"
#include "Class.h"

using namespace BWAPI;

class Ref
{
public:
	Ref(void);
	~Ref(void);
	static void onStart();
	static void setPermInputs();
	static int classAmt() {return clas.size();}
	static Class* getClass(unsigned int n) {return clas[n];}
	static Class* getClass(UnitType*);
	static bool isCore(UnitType*);
	static double getDangerVal(UnitType* type){return getClass(type)->getDangerVal();}
	static double getKillVal(UnitType* type){return getClass(type)->getKillVal();} 
	static bool isHarvesting(Order);
	static int updates();
	static void incUpdates();
	static bool hasSoldiers;
	static double gasToMinValRatio() {return Class::gasToMinValRatio;}
private:
	static int updateAmt;
	static std::vector<Class*> clas;
};

