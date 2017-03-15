#include "Economy.h"
#include <iostream>

std::vector<Base*> Economy::base=std::vector<Base*>();
std::vector<Unit*> Economy::workForce=std::vector<Unit*>();
std::vector<Unit*> Economy::bldg=std::vector<Unit*>();
std::list<UnitType*> Economy::buildQu=std::list<UnitType*>();
std::list<BuildOrder*> Economy::neededBldg=std::list<BuildOrder*>();
std::vector<Unit*> Economy::gateway=std::vector<Unit*>();
std::vector<Unit*> Economy::robotics=std::vector<Unit*>();
std::vector<Unit*> Economy::stargate=std::vector<Unit*>();
int Economy::lastGathMin=0;
int Economy::lastGathGas=0;
Net* Economy::net;
Army* Economy::army;
int Economy::birthOfLastBase=0;
bool Economy::pylonWarning=false;
bool Economy::pylonInProgress=false;
bool Economy::buildingAssm=false;

Economy::Economy(void)
{
	
}
void Economy::start()
{
	net=NetHandler::initNet("econ.txt",Net::Econ);
	std::set<Unit*> unit=Broodwar->self()->getUnits();

	for (std::set<Unit*>::iterator it=unit.begin();it!=unit.end();it++)
	{
		if ((*it)->getType()==UnitTypes::Protoss_Nexus)
		{
			base.push_back(new Base(*it));
		}
	}

	for (std::set<Unit*>::iterator it=unit.begin();it!=unit.end();it++)
	{
		if ((*it)->getType().isWorker())
		{
			workForce.push_back((*it));
			(*it)->gather(base[0]->getMinerals(workForce.size()));
		}
	}
	
}
void Economy::check()
{
	for (unsigned int n=0;n<base.size();n++)
	{
		base[n]->check();
	}
	for (unsigned int n=0;n<workForce.size();n++)
	{
		if (!(workForce[n])->exists())
		{
			Util::fprint((string)"RIP worker # ",(int)n,false);
			workForce.erase(workForce.begin()+n);
		}
		else if (workForce[n]->isIdle())
		{
			workForce[n]->gather(getNrstBase(workForce[n]->getPosition())->getRandMin());
		}

	}
}
void Economy::setPermInputs()
{
	for (unsigned int n=0;n<base.size();n++)
	{
		base[n]->setPermInputs();
	}
}
void Economy::update()
{
	check();
	lastGathMin=Broodwar->self()->gatheredMinerals();
	lastGathGas=Broodwar->self()->gatheredGas();
	bldgUpdate();
	netUpdate();
	pylonWarning=false;
	for (unsigned int n=0;n<base.size();n++)
	{
		if (base[n]->updateNet())//if buildAssm fired
		{
			Util::fprint("base says add assm");
			neededBldg.push_back(new BuildOrder(UnitTypes::Protoss_Assimilator,((base[n]->getAssm())->getPosition())));
			buildingAssm=true;
		}
	}
	Util::debug("econ updated");
}
void Economy::updateInputs()
{
	for (unsigned int n=0;n<net->inputAmt();n++)
	{
		if (net->getInput(n)->getName()=="minerals")
		{
			net->getInput(n)->setCurVal(Broodwar->self()->minerals());
		}
		else if (net->getInput(n)->getName()=="gas")
		{
			net->getInput(n)->setCurVal(Broodwar->self()->gas());
		}
		else if (net->getInput(n)->getName()=="workerAmt")
		{
			net->getInput(n)->setCurVal(workForce.size());
		}
		else if (net->getInput(n)->getName()=="amtBases")
		{
			net->getInput(n)->setCurVal(base.size());
		}
		else if (net->getInput(n)->getName()=="lifetimeMin")
		{
			net->getInput(n)->setCurVal(Broodwar->self()->gatheredMinerals() +  Broodwar->self()->refundedMinerals());
		}
		else if (net->getInput(n)->getName()=="lifetimeGas")
		{
			net->getInput(n)->setCurVal(Broodwar->self()->gatheredGas() +  Broodwar->self()->refundedGas());
		}
		else if (net->getInput(n)->getName()=="food")
		{
			net->getInput(n)->setCurVal(Broodwar->self()->supplyTotal()/2.0);
		}
		else if (net->getInput(n)->getName()=="foodUsed")
		{
			net->getInput(n)->setCurVal(Broodwar->self()->supplyUsed()/2.0);// bwapi returns supply as double the number
		}
		else if (net->getInput(n)->getName()=="pylonWarning")
		{
			net->getInput(n)->setCurVal(pylonWarning);
		}
		else if (net->getInput(n)->getName()=="pylonInProgress")
		{
			net->getInput(n)->setCurVal(pylonInProgress);
		}
		else if (net->getInput(n)->getName()=="buildingAssm")
		{
			net->getInput(n)->setCurVal(buildingAssm);
		}
	}
}
void  Economy::netUpdate()
{
	for (unsigned int n=0;n<net->outputAmt();n++)
	{
		if (net->getOutput(n)->getName()=="buildPylon")
		{
			if (net->getOutput(n)->get()==1)
			{
				neededBldg.push_back(new BuildOrder(UnitTypes::Protoss_Pylon));
				pylonInProgress=true;
			}
		}
	}
	//Util::fprint("} econ net");
}
void Economy::bldgUpdate()
{
	bool bldg=(neededBldg.size()>0);
	if (bldg)
	{
		Util::fprint("finding urg5's");
		for (std::list<BuildOrder*>::iterator b=neededBldg.begin();b!=neededBldg.end();b++)
		{
			if ((*b)->getUrgency() ==5)
			{
				Util::fprint("building urg5");
				if (buildBldg())
				{
					if (neededBldg.size()==0)
						bldg=false;
				}
				else bldg=false;
			}
		}
	}
	while (bldg)
	{
		if ((*neededBldg.begin())->getUrgency()==1)//yields to all else
		{
			bool others=false;
			for (std::list<BuildOrder*>::iterator b=neededBldg.begin();b!=neededBldg.end();b++)
			{
				if ((*b)->getUrgency() >2)
				{
					others=true;
					if (buildBldg(b))
					{
						if (neededBldg.size()==0)
							bldg=false;
						b=neededBldg.end();
					}
					else bldg=false;
				}
			}
			if (!others)
			{
				buildTroops();
				if (buildBldg())
				{
				 	if (neededBldg.size()==0)
						bldg=false;
				}
				else bldg=false;
			}
		}
		else if ((*neededBldg.begin())->getUrgency()==2)//yields to all but troops
		{
			bool others=false;
			for (std::list<BuildOrder*>::iterator b=neededBldg.begin();b!=neededBldg.end();b++)
			{
				if ((*b)->getUrgency() >2)
				{
					others=true;
					if (buildBldg(b))
					{
						if (neededBldg.size()==0)
							bldg=false;
						b=neededBldg.end();
					}
					else bldg=false;
				}
			}
			if (!others)
			{
				if (buildBldg())
				{
					if (neededBldg.size()==0)
						bldg=false;
				}
				else bldg=false;
				buildTroops();
			}
		}
		else if ((*neededBldg.begin())->getUrgency()==3)
		{
			Util::fprint("bldgUpdate(): building an urg3 bldg");
			if (buildBldg())
			{
				Util::fprint("urg3 bldg built");
				if (neededBldg.size()==0)
					bldg=false;
			}
			else bldg=false;
			buildTroops();
		}
		else if ((*neededBldg.begin())->getUrgency()==4)
		{
			if (buildBldg())
			{
				if (neededBldg.size()==0)
					bldg=false;
				buildTroops();
			}
			else bldg=false;
		}
		else if ((*neededBldg.begin())->getUrgency()==5)
		{//shouldve already been gotten
			Util::fprint("ERR MISTAKE: bldgUpdate() did not find an urg5 buildOrder the first time.");
			if (buildBldg())
			{
				if (neededBldg.size()==0)
					bldg=false;
				buildTroops();
			}
			else bldg=false;
		}
		else
		{
			Util::fprint("ERROR!  urgency of bldg is ",(*neededBldg.begin())->getUrgency());
			(*neededBldg.begin())->setUrgency(3);
		}
	}//to while(bldg)
}
bool Economy::buildBldg(std::list<BuildOrder*>::iterator b)
{
	if (ownEnoughResource((*b)->getType()))
	{
		if (!(*b)->hasPos())
		{
			bool sucs=Area::getArea(base[0]->getBase()->getPosition())->build((*b)->getType(),	getClosestWorker(base[0]->getBase()->getPosition()));
			if (sucs)
			{
				neededBldg.erase(b);
			}
			return sucs;
		}
		else
		{
			if ((getClosestWorker((*b)->getPos().x(),(*b)->getPos().y()))->build(Util::posToTile((*b)->getPos()),*(*b)->getType()))
			{
				delete *b;
				neededBldg.erase(b);
				return true;
			}
			else return false;
		}
	}
	else return false;
}
void Economy::buildTroops()
{
	while (buildQu.size()>0)
	{
		if ((*buildQu.begin())->whatBuilds().second==1)//if built by a building
		{
			if (ownEnoughResource(*buildQu.begin()))
			{
				if ((getFactory((*buildQu.begin())->whatBuilds().first))->train(*(*buildQu.begin())))
				{
					buildQu.pop_front();
				}
			}
		}
		else if ((*buildQu.begin())->whatBuilds().second!=0)//if built by a morphing unit
		{
			std::pair<Unit*,Unit*> morpher=army->getTemplarToMorph(
				((*buildQu.begin())->whatBuilds().first)==UnitTypes::Protoss_Dark_Templar);
		//	(morpher.first)->mergeWithOtherGuy();
		}

	}
}
void Economy::addOrder(UnitType* order)
{
	if (order->canMove())
		buildQu.push_back(order);
	else
		neededBldg.push_back(new BuildOrder(order));
}
void Economy::addOrder(UnitType* order,int howMany)
{
	for (int n=0;n<howMany;n++)
	{
		addOrder(order);
	}
}
void Economy::addOrder(BuildOrder* order)
{
	if (!order->getType()->canMove())
		neededBldg.push_back(order);
	else 
	{
		buildQu.push_back(order->getType());
		delete order;
	}
}
void Economy::addOrder(Position place, UnitType* build)
{
	if (!build->canMove())
		neededBldg.push_back(new BuildOrder(build,place));
	else
		buildQu.push_back(build);
}
void Economy::clearQu()
{
	buildQu.clear();
}
void Economy::clearBldgQu()
{
	for (std::list<BuildOrder*>::iterator del=neededBldg.begin();del!=neededBldg.end();del++)
		delete *del;
	neededBldg.clear();
}
void Economy::removeAllOfType(UnitType* remove)
{
	if (remove->canMove())// if (!building)
		for (std::list<UnitType*>::iterator it=buildQu.begin();it!=buildQu.end();it++)
		{
			if ((*it)->getID()==remove->getID())
			{
				buildQu.erase(it);
			}
		}
	else
		for (std::list<BuildOrder*>::iterator it=neededBldg.begin();it!=neededBldg.end();it++)
		{
			if ((*it)->getType()->getID()==remove->getID())
			{
				delete (*it);
				neededBldg.erase(it);
			}
		}
}
void Economy::addWorker(Unit* worker)
{
	workForce.push_back(worker);
	Base* nrstBase=base[0];
	int clostDist=Util::roughDist(base[0]->getBase()->getPosition(),worker->getPosition());
	for (unsigned int n=1;n<base.size();n++)
	{
		int dist=Util::roughDist(base[0]->getBase()->getPosition(),worker->getPosition());
		if (dist<clostDist)
		{
			clostDist=dist;
			nrstBase=base[n];
		}
	}
	worker->gather(nrstBase->getMinerals(workForce.size()),false);
}
void Economy::addFactory(Unit* what)
{
	if ((what)->getType()==UnitTypes::Protoss_Nexus)
		addBase(what);
	else if ((what)->getType()==UnitTypes::Protoss_Gateway)
		gateway.push_back(what);
	else if ((what)->getType()==UnitTypes::Protoss_Robotics_Facility)
		robotics.push_back(what);
	else if ((what)->getType()==UnitTypes::Protoss_Stargate)
		stargate.push_back(what);
	else
		Util::fprint("WARNING!: econ::addFactory: "+what->getType().getName()+" passed in");
}
Unit* Economy::getFactory(UnitType type)
{
	std::vector<Unit*>* factory;
	if (type==UnitTypes::Protoss_Gateway)
		factory=&gateway;
	else if (type==UnitTypes::Protoss_Robotics_Facility)
		factory=&robotics;
	else if (type==UnitTypes::Protoss_Stargate)
		factory=&stargate;
	else if (type==UnitTypes::Protoss_Nexus)
	{
		int lowestTime=(base[0])->getBase()->getRemainingTrainTime();
		Unit* who=base[0]->getBase();
		for (unsigned int n=1;n<base.size();n++)
		{
			if (base[n]->getBase()->getRemainingTrainTime()<lowestTime)
			{
				lowestTime=base[n]->getBase()->getRemainingTrainTime();
				who=base[n]->getBase();
			}
		}
		return who;
	}
	else
	{
		Util::fprint("warning! econ: getFactory() parameter is not a factory, its a "+type.getName());
		factory=&gateway;
	}

	int lowestTime=(factory->at(0))->getRemainingTrainTime();
	Unit* who=factory->at(0);
	for (unsigned int n=1;n<factory->size();n++)
	{
		if ((factory->at(n))->getRemainingTrainTime()<lowestTime)
		{
			lowestTime=(factory->at(n))->getRemainingTrainTime();
			who=factory->at(n);
		}
	}
	return who;
}
void Economy::addBldg(Unit* add)
{
	if (add->getType()==UnitTypes::Protoss_Pylon)
		pylonInProgress=false;
	else if (add->getType()==UnitTypes::Protoss_Assimilator)
		buildingAssm=false;
	else if (add->getType().isResourceDepot())
		addBase(add);
	else if (add->getType().canProduce())
		addFactory(add);
	bldg.push_back(add);
}
void Economy::addBase(Unit* add)
{
	for (unsigned int n=0;n<base.size();n++)
	{
		if (add->getID()==base[n]->getBase()->getID())
			return;
	}
	birthOfLastBase=Ref::updates();
	base.push_back(new Base(add));
	Broodwar->printf("Look daddy I'm building a base!");

}
Base* Economy::getNrstBase(Position pos)
{
	double closeDist=9999999;
	Base* closest=base[0];
	for (unsigned int n=1;n<base.size();n++)
	{
		if (base[n]->getBase()->isCompleted() && base[n]->getBase()->exists())
		{
			//heuristic distance: x1-x2+ y1-y2
			double dist=Util::roughDist(base[n]->getBase()->getPosition(),pos);
			if (dist>closeDist)
			{
				closeDist=dist;
				closest=base[n];
			}
		}
	}
	return closest;
}
int Economy::getWorkerAmt() {
	return workForce.size();
}
Unit* Economy::getClosestWorker(int x,int y)
{
	double closeDist=9999999;
	Unit* closest=workForce[0];
	for (unsigned int n=0;n<workForce.size();n++)
	{
		if (workForce[n]->isCompleted() && workForce[n]->exists())
		{
			//heuristic distance: x1-x2+ y1-y2
			double dist=Util::roughDist(workForce[n]->getPosition().x(),workForce[n]->getPosition().y(),x,y);
			if ((workForce[n])->getOrder()==Orders::None)
				dist-=250;
			if (dist>closeDist)
			{
				closeDist=dist;
				closest=workForce[n];
			}
		}
	}
	return closest;
}
bool Economy::ownEnoughResource(UnitType* build)
{
	if (build->mineralPrice() > Broodwar->self()->minerals()) return false;
	if (build->gasPrice() > Broodwar->self()->gas()) return false;
	if (build->supplyRequired()+Broodwar->self()->supplyUsed() > Broodwar->self()->supplyTotal()) 
	{
		pylonWarning=true;
		return false;
	}
	else return true;
}
Economy::~Economy(void)
{
}
