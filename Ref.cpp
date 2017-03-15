#include "Ref.h"
#include "Util.h"

int Ref::updateAmt =0;
bool Ref::hasSoldiers=true;

std::vector<Class*> Ref::clas=std::vector<Class*>();

Ref::Ref()
{

}
void Ref::onStart()
{
	std::set<Player*> player=Broodwar->getPlayers();
	bool terran=false,zerg=false;
	for (std::set<Player*>::iterator ply=player.begin();ply!=player.end();ply++)
	{
		if ((*ply)->isEnemy(Broodwar->self()))
		{
			if ((*ply)->getRace()==Races::Zerg)
				zerg=true;
			else if ((*ply)->getRace()==Races::Terran)
				terran=true;
			else if ((*ply)->getRace()==Races::Unknown)
			{
				zerg=true;
				terran=true;
			}
		}
		if (zerg&&terran)
			break;
	}
	std::set<UnitType> all=UnitTypes::allUnitTypes();
	for (std::set<UnitType>::iterator ut=all.begin();ut!=all.end();ut++)
	{
		if (  ut->getRace()==Races::Protoss
			||(ut->getRace()==Races::Terran && terran)
			||(ut->getRace()==Races::Zerg && zerg) )
		{
			if (!ut->isHero()&& !ut->isSpecialBuilding()&& !ut->isAddon() && !ut->isSpell() && ut->getID()!=UnitTypes::Terran_Siege_Tank_Siege_Mode.getID())
			{
				clas.push_back(new Class(*ut));
			}
		}
	}
}
void Ref::setPermInputs()
{
	for (unsigned int n=0;n<clas.size();n++)
	{
		clas[n]->setPermInputs();
	}
	Util::debug("classes updated");
}
Class* Ref::getClass(UnitType* type)
{
	for (unsigned int n=0;n<clas.size();n++)
	{
		if (type->getID()==clas[n]->getType().getID())
			return clas[n];
	}
	Util::fprint("warning! someones looking for the class of "+type->getName()+" which isnt there");
	return new Class();
}
bool Ref::isCore(UnitType* type)
{
	for (unsigned int n=0;n<clas.size();n++)
	{
		if (type->getID()==clas[n]->getType().getID())
			return true;
	}
	return false;
}
bool Ref::isHarvesting(Order order)
{
	if (order==Orders::Harvest1) return true;
	if (order==Orders::Harvest2) return true;
	if (order==Orders::Harvest3) return true;
	if (order==Orders::Harvest4) return true;
	if (order==Orders::HarvestGas) return true;
	if (order==Orders::MiningMinerals) return true;
	if (order==Orders::MoveToGas) return true;
	if (order==Orders::MoveToMinerals) return true;
	if (order==Orders::WaitForGas) return true;
	if (order==Orders::WaitForMinerals) return true;
	if (order==Orders::ReturnGas) return true;
	if (order==Orders::ReturnMinerals) return true;
	return false;
}

int Ref::updates() {
	return updateAmt;
}
void Ref::incUpdates() {
	updateAmt++;
	hasSoldiers=false;
}
Ref::~Ref(void)
{
}
