#include "Class.h"

double Class::gasToMinValRatio=1.6;

class Army;

Class::Class()
{
}
Class::Class(const UnitType unittype)
{
//	Util::fprint("&unittype now=",(void*)&unittype);
	type=(unittype);
	if (NetHandler::fileExists("unittype"+type.getName()+".txt"))
		net=NetHandler::initNet("unittype"+type.getName()+".txt");
	else
		net=NetHandler::initNet("unittype.txt",Net::UnitType);
}
void Class::setPermInputs()
{
//	Util::fprint("setting perminput of "+type.getName());
	for (unsigned int n=0;n<net->inputAmt();n++)
	{
		if (net->getInput(n)->getName()=="DPS")
		{
			double groundDPS=0,airDPS=0;
			if (type.groundWeapon().damageCooldown()!=0)
				groundDPS=(double)type.groundWeapon().damageAmount()/(double)type.groundWeapon().damageCooldown();
			if (type.airWeapon().damageCooldown()!=0)
				airDPS=(double)type.airWeapon().damageAmount()/(double)type.airWeapon().damageCooldown();
			net->getInput(n)->setState(groundDPS+airDPS);
		}
		else if (net->getInput(n)->getName()=="abilitiesScore")
		{
			double ablScore=0;
			std::set<TechType> spells=type.abilities();
			if (type.canProduce())
			{
				if (type.getID()==UnitTypes::Protoss_Carrier)
					ablScore+=100;//todo: find real value
				else if (type.getID()==UnitTypes::Protoss_Reaver)
					ablScore+=60;//todo: find real value
			}
			for each (TechType abl in spells)
			{
				if (abl.getWeapon()!=WeaponTypes::None)
				{
					double spellVal=0;
					spellVal+=abilitySpecialDangerVal(abl);
					spellVal=(double)abl.getWeapon().damageAmount()/(double)abl.getWeapon().damageCooldown();
					spellVal/=((double)abl.energyUsed()/(double)type.maxEnergy())+(((double)abl.mineralPrice()+(double)abl.gasPrice()*gasToMinValRatio)/50);
					ablScore+=spellVal;
				}
			}
			net->getInput(n)->setState(0);
		}
		else if (net->getInput(n)->getName()=="econScore")
		{
			//todo: find better values for these
			double econScore=0;
			if (type.canProduce())
				if (!type.canMove())//excludes reavers and carriers
					econScore+=300;
			if (type.isResourceDepot())
				econScore+=700;
			if (type.isAddon())
				econScore+=50;
			else if (type.isBuilding())
				econScore+=100;
			if (type.isWorker())
				econScore+=100;
			if (type.isDetector())
				econScore+=100;
			econScore+=type.mineralPrice()+type.gasPrice()*gasToMinValRatio;
			econScore+=type.supplyProvided()*10;//remember supplyProvided() returns double
			if (type==UnitTypes::Protoss_Pylon)
				econScore+=150;//for the benefits of energy fields
			else if (type==UnitTypes::Zerg_Creep_Colony)
				econScore+=100;
			net->getInput(n)->setCurVal(econScore);
		}
		else if (net->getInput(n)->getName()=="minCost")
		{
			net->getInput(n)->setCurVal(type.mineralPrice());
		}
		else if (net->getInput(n)->getName()=="gasCost")
		{
			net->getInput(n)->setCurVal(type.gasPrice());
		}
		else if (net->getInput(n)->getName()=="permCloak")
		{
			net->getInput(n)->setCurVal(type.hasPermanentCloak());
		}
		else if (net->getInput(n)->getName()=="shields")
		{
			net->getInput(n)->setCurVal(type.maxShields());
		}
		else if (net->getInput(n)->getName()=="health")
		{
			net->getInput(n)->setCurVal(type.maxHitPoints());
		}
		else if (net->getInput(n)->getName()=="range")
		{
			net->getInput(n)->setCurVal(Util::getWeaponRange(type));
		}
		else if (net->getInput(n)->getName()=="regenHealth")
		{
			net->getInput(n)->setCurVal(type.regeneratesHP());
		}
		else if (net->getInput(n)->getName()=="suicide")
		{
			net->getInput(n)->setCurVal(isSuicidal());
		}
	}
	for (unsigned int n=0;n<net->outputAmt();n++)
	{
		if (net->getOutput(n)->getName()=="dangerVal")
		{
			dangerVal=net->getOutput(n)->get();
			Util::debug("          danger val of "+type.getName()+" =",dangerVal);
		}
		else if (net->getOutput(n)->getName()=="killVal")
		{
			killVal=net->getOutput(n)->get();
			Util::debug("            kill val of "+type.getName()+" =",killVal);
		}
	}
}
void Class::updateInputs()
{
	for (unsigned int n=0;n<net->inputAmt();n++)
	{
		if (net->getInput(n)->getName()=="ownUse")
		{

		}
		else if (net->getInput(n)->getName()=="observedEnUse")
		{

		}
	}
}
bool Class::isSuicidal(UnitType* ut)
{
	if (ut->getID()==UnitTypes::Protoss_Scarab) return true;
	else if (ut->getID()==UnitTypes::Zerg_Infested_Terran) return true;
	else if (ut->getID()==UnitTypes::Zerg_Scourge) return true;
	return false;
}
bool Class::isTrim(UnitType* ut)
{
	if (ut->isWorker()) return true;
	if (ut->canAttack()) return false;//except workers
	if (ut->isSpellcaster()) return true;
	if (!ut->canMove()) return true;//except shield battery,comsat station, and turret/cannon/spore colony/sunken colony
}
double Class::abilitySpecialDangerVal(BWAPI::TechType abl)
{//the danger the ability creates, excluding direct damage
		 if (abl.getID()== TechTypes::Archon_Warp.getID()) return 0;
	else if (abl.getID()==TechTypes::Burrowing.getID()) return 50;
	else if (abl.getID()==TechTypes::Cloaking_Field.getID()) return 150;
	else if (abl.getID()==TechTypes::Consume.getID()) return 0;
	else if (abl.getID()==TechTypes::Dark_Archon_Meld.getID()) return 0;
	else if (abl.getID()==TechTypes::Dark_Swarm.getID()) return 0;
	else if (abl.getID()==TechTypes::Defensive_Matrix.getID()) return 0;
	else if (abl.getID()==TechTypes::Disruption_Web.getID()) return 0;
	else if (abl.getID()==TechTypes::EMP_Shockwave.getID()) return 0;
	else if (abl.getID()==TechTypes::Ensnare.getID()) return 0;
	else if (abl.getID()==TechTypes::Feedback.getID()) return 20;
	else if (abl.getID()==TechTypes::Hallucination.getID()) return 0;
	else if (abl.getID()==TechTypes::Healing.getID()) return 0;
	else if (abl.getID()==TechTypes::Infestation.getID()) return 0;
	else if (abl.getID()==TechTypes::Irradiate.getID()) return 0;
	else if (abl.getID()==TechTypes::Lockdown.getID()) return 0;
	else if (abl.getID()==TechTypes::Lurker_Aspect.getID()) return 0;
	else if (abl.getID()==TechTypes::Maelstrom.getID()) return 0;
	else if (abl.getID()==TechTypes::Mind_Control.getID()) return 150;
	else if (abl.getID()==TechTypes::Nuclear_Strike.getID()) return 500;
	else if (abl.getID()==TechTypes::Optical_Flare.getID()) return 0;
	else if (abl.getID()==TechTypes::Parasite.getID()) return 150;
	else if (abl.getID()==TechTypes::Personnel_Cloaking.getID()) return 70;
	else if (abl.getID()==TechTypes::Plague.getID()) return 0;
	else if (abl.getID()==TechTypes::Psionic_Storm.getID()) return 0;
	else if (abl.getID()==TechTypes::Recall.getID()) return 0;
	else if (abl.getID()==TechTypes::Restoration.getID()) return 0;
	else if (abl.getID()==TechTypes::Scanner_Sweep.getID()) return 30;
	else if (abl.getID()==TechTypes::Spawn_Broodlings.getID()) return 0;
	else if (abl.getID()==TechTypes::Spider_Mines.getID()) return 0;
	else if (abl.getID()==TechTypes::Stasis_Field.getID()) return 0;
	else if (abl.getID()==TechTypes::Stim_Packs.getID()) return 20;
	else if (abl.getID()==TechTypes::Tank_Siege_Mode.getID()) return 40;
	else if (abl.getID()==TechTypes::Yamato_Gun.getID()) return 50;
	else return 0;
}
Class::~Class()
{
}
