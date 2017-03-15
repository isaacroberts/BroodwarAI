#include <iostream>
#include "AIforSSCAI.h"

using namespace BWAPI;

void AIforSSCAI::onStart()
{
	AIforSSCAI();		
	Broodwar->enableFlag(Flag::UserInput);
//	Broodwar->enableFlag(Flag::CompleteMapInformation);
	Broodwar->setLocalSpeed(1);
//	Broodwar->printf("black sheep wall");
	Util::setup();
	goalnet=NetHandler::initNet("goal.txt",Net::Goal);
	army=new Army();
	Area::init();
	Ref::onStart();
	Economy::start();
	
	Util::fprint("connecting existing nets");
	NetHandler::connectAll();
	Util::fprint("nets loaded");
	Economy::setPermInputs();
	Ref::setPermInputs();
	Area::setAllPermInputs();
	Util::fprint("Running");
	Broodwar->printf("Running");
}
void AIforSSCAI::onFrame()
{
	Ref::incUpdates();
	if (Broodwar->isPaused())
		return;
	Broodwar->drawTextScreen(7,7,"update #%i",Ref::updates());
	Util::fprint();
	Util::debug("-----------------------------");
	Util::debug("update = ",Ref::updates());
	NetHandler::resetNodes();
	Util::debug("Updating inputs");
	goalnetUpdateInputs(); 
	Economy::updateInputs();
	Area::updateAllInputs();
	Util::debug("Getting outputs");
	Economy::update();
	Area::updateAll();
	army->update();
	Util::debug("All updated");
	if (Ref::updates()%30==0)
	{
		Util::checkFileLength();
		Util::fprint("file length checked");
	}
	/*
	catch (exception e)
	{
		Broodwar->printf("Error : ");
		Broodwar->printf(e.what());
		Util::fprint("ERROR! AIforSSCAI::update ",false);
		Util::fprint(e.what());
	}
	*/
}
void AIforSSCAI::onSendText(string text)
{
	Util::fprint("text sent:"+text);
	if (text=="speed 0")
	{
		Broodwar->setLocalSpeed(0);
	}
	else if (text=="speed 5")
		Broodwar->setLocalSpeed(5);
	else Broodwar->printf("text sent : %s",text);
}
void AIforSSCAI::onUnitCreate(Unit* newUnit)
{
	
}
void AIforSSCAI::onUnitComplete(Unit* newUnit)
{
	//Util::fprint("Unit created : "+newUnit->getType().getName());
	Area::giveOccupant(newUnit);
	if (newUnit->getPlayer()->getID()==Broodwar->self()->getID())
	{
		if (newUnit->getType().isWorker())
		{
			Economy::addWorker(newUnit);
		}
		else if (!newUnit->getType().canMove())
		{
			Economy::addBldg(newUnit);
		}
		else// if (newUnit->getType().canMove() && !newUnit->getType().isWorker())
		{
			army->addUnit(newUnit);
		}
	}
}
void AIforSSCAI::onUnitDestroy(Unit* vic)
{
	if (vic->getPlayer()->isEnemy(Broodwar->self()))
	{
		if (vic->getType().canMove())
		{
			((Goal*)goalnet->getNode("g_killEnUnits"))->getMeet()->addCurVal(/*UnitTypeHandler::getKillVal(vic->getType())*/vic->getType().destroyScore());
			if (vic->getType().isWorker())
			{
				goalnet->getNode("g_killEnBase")->addCurVal(.15);
			}
		}
		else
		{
			((Goal*)goalnet->getNode("g_killEnBldg"))->getMeet()->addCurVal(/*UnitTypeHandler::getKillVal(vic->getType())*/vic->getType().destroyScore());
			if (vic->getType().isResourceDepot())
				((Goal*)goalnet->getNode("g_killEnBldg"))->getMeet()->addCurVal(4);
		}
	}
	else if (vic->getPlayer()->getID()==Broodwar->self()->getID())
	{
		if (vic->getType().canMove() && !vic->getType().isWorker())
		{
			army->unitDead(vic);
		}
	}
}
void AIforSSCAI::onUnitDiscover(Unit* foundUnit)
{
	Area::getArea(foundUnit->getPosition())->addOccupant(foundUnit);
}
void AIforSSCAI::onUnitMorph(Unit* morphed)
{
	Util::fprint("Unit morphed: "+morphed->getType().getName());	
}
void AIforSSCAI::onEnd(bool win)
{
	Util::fprint("Win = ",false);
	Util::fprint(win);
	Util::fprint();
	((Goal*)goalnet->getNode("g_win"))->getMeet()->setCurVal(win);
	goalnet->getNode("g_win")->get();
	NetHandler::excommunicate(goalnet);
}
void AIforSSCAI::goalnetUpdateInputs()
{
	for (unsigned int n=0;n<goalnet->inputAmt();n++)
	{
		if (goalnet->getInput(n)->getName()=="harvestRes")
		{
			double gatheredRes=econ->getGathMin()+ econ->getGathGas()*Ref::gasToMinValRatio();
			((Goal*)goalnet->getInput(n))->getMeet()->setCurVal(gatheredRes);
		}
		else if (goalnet->getInput(n)->getName()=="buildBase")
		{
			((Goal*)goalnet->getInput(n))->getMeet()->setCurVal(Economy::getAgeOfLastBase()<=1);

		}
	}
}