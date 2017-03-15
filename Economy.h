#ifndef ECONOMY_H
#define	ECONOMY_H

#include <vector>
#include <list>
#include <string>
#include <BWAPI.h>

#include "Area.h"
#include "BuildOrder.h"
#include "Util.h"
#include "Army.h"
#include "Base.h"

using namespace BWAPI;

class Economy
{
public:
	Economy(void);
	~Economy(void);
	static void showArmy(Army* military) {
		army=military;
	}
	static void start();
	static void check();
	static void setPermInputs();
	static void updateInputs();
	static void update();
	static void netUpdate();
	static void bldgUpdate();
	static bool buildBldg() {
		return buildBldg(neededBldg.begin());
	}
	static bool buildBldg(std::list<BuildOrder*>::iterator);
	static void buildTroops();
	static void addOrder(BuildOrder*);
	static void addOrder(UnitType*);
	static void addOrder(UnitType*,int howMany);
	static void addOrder(Position,UnitType*);
	static void clearQu();
	static void clearBldgQu();
	static void removeAllOfType(UnitType*);
	static void addWorker(Unit*);
	static void addFactory(Unit*);
	static Unit* getFactory(UnitType);
	static void addBase(Unit*);
	static Base* getBase(unsigned int n) {return base[n];}
	static Base* getNrstBase(Position pos);
	static int getWorkerAmt();
	static Unit* getClosestWorker(Position pos) {return getClosestWorker(pos.x(),pos.y());}
	static Unit* getClosestWorker(int x,int y);
	static int getGathGas() {return Broodwar->self()->gatheredGas()-lastGathGas;}
	static int getGathMin() {return Broodwar->self()->gatheredMinerals()-lastGathMin;}
	static int getAgeOfLastBase() { return Ref::updates()-birthOfLastBase;}
	static bool ownEnoughResource(UnitType*);
	static void addBldg(Unit*);
private: 
	static Army* army;
	static std::vector<Base*> base;
	static std::vector<Unit*> workForce;
	static std::vector<Unit*> gateway,robotics,stargate;
	static std::list<UnitType*> buildQu; //unit build queue
	static std::list<BuildOrder*> neededBldg;
	static std::vector<Unit*> bldg;
	static Net* net;
	
	static int birthOfLastBase;
	static int lastGathMin,lastGathGas;
	static bool pylonWarning,pylonInProgress,buildingAssm;
};
#endif
