#include "Army.h"

Army::Army(void)
{
	army=std::vector<Troop*>();
	//todo: add army net
//	net=NetHandler::initNet("army.txt",Net::Army);
//	group=std::vector<CmndGroup*>();
}
void Army::update()
{
	Ref::hasSoldiers=(army.size()!=0);
	for (unsigned int n=0;n<army.size();n++)
	{
		army[n]->autonomous();
	}
	unitUsageChanged=true;
}
void Army::updateInputs()
{
}
void Army::updateNet()
{
}
void Army::addUnit(Unit* who)
{
	army.push_back(new Troop(who));
	unitUsageChanged=true;
}
void Army::unitDead(Unit* vic)
{
	for (unsigned int n=0;n<army.size();n++)
	{
		if ((vic)->getID()==army[n]->getUnit()->getID())
		{
			army.erase(army.begin()+n);
			return;
		}
	}
	unitUsageChanged=true;
}
int Army::getUsage(UnitType type)
{
	int usage=0;
	for each(Troop* dood in army)
	{
		if (dood->getUnit()->getType().getID()==type.getID())
			usage++;
	}
	return usage;
}
/*
CmndGroup* Army::getGroup(int which) 
{
//	if (which<group.size())
		return group[which];
//	else return group[0];
}
CmndGroup* Army::addGroup()
{
	group.push_back(new CmndGroup());
	return group[group.size()-1]->getID();
}
void Army::removeGroup(int which)
{
	delete *(group.begin()+which);
	group.erase(group.begin()+which);
}
void Army::removeGroup(CmndGroup* which)
{
	for (unsigned int n=0;n<group.size();n++)
	{
		if (which->getID()==group[n]->getID())
		{
			delete which;
			group.erase(group.begin()+n);
			return;
		}
	}
}
*/
std::pair<Unit*,Unit*> Army::getTemplarToMorph(bool isDark)
{
	std::pair<Unit*,Unit*> morpher;
	int amt=0;
	for (unsigned int n=army.size()-1;n>=0;n--)//iterate backwards-find the last two to be created
	{
		if (amt==2)
			return morpher;
		
		if (isDark)
			if ((army[n])->getUnit()->getType()==UnitTypes::Protoss_Dark_Templar)
			{
				if (amt==0)
					morpher.first=army[n]->getUnit();
				else
					morpher.second=army[n]->getUnit();
				amt++;
			}
		else
			if ((army[n])->getUnit()->getType()==UnitTypes::Protoss_High_Templar)
			{
				if (amt==0)
					morpher.first=army[n]->getUnit();
				else
					morpher.second=army[n]->getUnit();
				amt++;
			}
	}
	std::pair<Unit*,Unit*> error;
	error.first=error.second=NULL;
	return error;
	//todo- add method find the two that are best in a position to morph instead
}
Army::~Army(void)
{
	for (unsigned int n=0;n<army.size();n++)
		delete army[n];
	NetHandler::excommunicate(net);
	//for (unsigned int n=0;n<group.size();n++)
	//	delete group[n];
}
