#ifndef ARMY_H
#define ARMY_H

#include <vector>
#include <BWAPI.h>
#include "NetHandler.h"
#include "Ref.h"
#include "Troop.h"

using namespace BWAPI;

class Army
{
public:
	Army(void);
	~Army(void);
	void update();
	void updateInputs();
	void updateNet();
	void addUnit(Unit* who);
	void unitDead(Unit*);
	bool hasUnitUsageChanged(){return unitUsageChanged;}
	int getUsage(UnitType);
//	std::vector<CmndGroup*> getCmndGroups() {
//		return group;  }
//	CmndGroup* getGroup(int);
//	CmndGroup* addGroup();
//	void removeGroup(int);
//	void removeGroup(CmndGroup*);
//	int groupAmt() {
//		return group.size(); }
	std::pair<Unit*,Unit*> getTemplarToMorph(bool isDark);
private:
	bool unitUsageChanged;
	std::vector<Troop*> army;
	Net* net;
//	std::vector<CmndGroup*> group;
};

#endif
