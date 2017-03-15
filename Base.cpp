#include "Base.h"

Base::Base()
{
}

Base::Base(Unit* thisBase)
{
	base=thisBase;
	std::set<Unit*> nearby=(base)->getUnitsInRadius(300);
	for (std::set<Unit*>::iterator it=nearby.begin();it!=nearby.end();it++)
	{
		if ((*it)->getType()==UnitTypes::Resource_Vespene_Geyser)
		{
			assm=(*it);
		}
		else if ((*it)->getType().isMineralField())
		{
			mineral.push_back((*it));
		}
	}
	Util::fprint("Initiallizing base net");
	net=NetHandler::initNet("base.txt",Net::Base);
	//NetHandler::connectNet(net);
}
void Base::setPermInputs()
{
	//for (unsigned int n=0;n<net->inputAmt();n++)
	{
		
	}
}
void Base::check()
{
	
}
void Base::updateInputs()
{
	//Util::fprint("base->net { ");
	int wkrsOnAssm=-1,wkrsOnMin=-1;
	if (Ref::updates()%7==0)
	{
		wkrsOnAssm=0;
		wkrsOnMin=0;
		std::set<Unit*> wkrs;
		wkrs=(base)->getUnitsInRadius(40);
		for (std::set<Unit*>::iterator it=wkrs.begin();it!=wkrs.end();it++)
		{
			if ((*it)->getType()==UnitTypes::Protoss_Probe)
			{
				if ((*it)->getPlayer()->getID()==Broodwar->self()->getID())
				{
					if ((*it)->isCarryingGas())
						wkrsOnAssm++;
					else if ((*it)->isCarryingMinerals())
	 					wkrsOnMin++;
				}
			}
		}
	}
	for (unsigned int n=0;n<net->inputAmt();n++)
	{
		Util::debug("base input "+net->getInput(n)->getName());
		if (net->getInput(n)->getName()=="assm?")
		{
			if (assm==NULL)
			{
				Util::fprint("WARNING! base::updateInputs: assm is null");
				net->getInput(n)->setCurVal(0);
			}
			else
			{
				net->getInput(n)->setCurVal((assm)->getType()==UnitTypes::Protoss_Assimilator);
			}
		}
		else if (net->getInput(n)->getName()=="wkrsOnAssm")
		{
			if (wkrsOnAssm==-1)
				net->getInput(n)->setState(net->getInput(n)->getLastState());
			else
				net->getInput(n)->setCurVal(wkrsOnAssm);
		}
		else if (net->getInput(n)->getName()=="wkrsOnMin")
		{
			if (wkrsOnMin==-1)
				net->getInput(n)->setState(net->getInput(n)->getLastState());
			else
				net->getInput(n)->setCurVal(wkrsOnMin);
		}
		else if (net->getInput(n)->getName()=="amtMinFlds")
		{
			net->getInput(n)->setCurVal((double)mineral.size());
		}
		else if (net->getInput(n)->getName()=="wkrInQu")
		{
			net->getInput(n)->setCurVal(base->getRemainingTrainTime()!=0);
		}
	}
}
bool Base::updateNet()
{
	bool buildAssm=false;
	for (unsigned int n=0;n<net->outputAmt();n++)
	{
		if (net->getOutput(n)->getName()=="buildWrkr")
		{
			if (net->getOutput(n)->get()>=1)
			{
				if (Broodwar->canMake(NULL,(UnitType)(UnitTypes::Protoss_Probe)))
				{
					if (!(base)->train(UnitTypes::Protoss_Probe))
					{
						Broodwar->printf("probe could not be built");
						Util::fprint("probe could not be built");
						net->getOutput(n)->setState(0);
					}
				}
			}
		}
		else if (net->getOutput(n)->getName()=="buildAssm")
		{
			buildAssm=(net->getOutput(n)->get()>=1);
		}
	}
	//Util::fprint("}  baseNet");
	return buildAssm;
}
Unit* Base::getMinerals(unsigned int ix)
{
	if ((unsigned int)ix<mineral.size())
		return mineral[ix];
	else return mineral[ix%mineral.size()];
}
Unit* Base::getRandMin()
{
	if (Ref::updates()==0)
		return mineral[rand()%mineral.size()];
	return mineral[Ref::updates()%mineral.size()];
}