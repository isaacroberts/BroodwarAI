#include <iostream>
#include <BWAPI.h>

#include "Goal.h"
//#include "Node.h"

using namespace std;

Goal::Goal() : Node()
{
	name="newgoal";
	setJob(Node::GoalDrive);
	setType(Node::Linear);

	meet=new Node();
	meet->setJob(Node::GoalMeet);
	meet->setType(Node::Flash);
	meet->setThresh(.5);
	meet->setMiscMod(.3);
	meetWeight=1;

	strive=new Node();
	strive->setJob(Node::GoalStrive);
	strive->setType(Node::Flash);
	strive->setThresh(.5);
	strive->setMiscMod(.05);
	striveWeight=.3;

	mem=new Node();
	mem->setJob(Node::ShortTermMem);
	mem->setType(Node::Set);
}

double Goal::get()
{
	if (checked)
	{
		return getState();
	}
	else if (visited)
		return lastState;
	visited=true;
	double meetVal=meet->get()*meetWeight;
	Util::debug("    "+meet->getName()+" s*w:",meetVal);
	if (meetVal>=.8)
	{
		meet->learnThresh(.1);
		transferMem();
		setCurVal(0);
		calcState(true);
		return getState();
	}
	else
	{
		double strv=strive->get()*striveWeight;
		Util::debug("    "+strive->getName()+" -> "+name+" s*w:",strv);
		addCurVal(strv);
		Node::get();
		if (meetVal>=0)
		{
			currentVal*=(1.0-meetVal);
			meet->learnThresh((1.0-meetVal)/-10.0);
		}
		else
		calcState(true);
		checked=true;
		Util::debug(name+" state= ",state);
		return getState();
	}
}
void Goal::transferMem()
{
	//BWAPI::Broodwar->printf("Goal met! : %s",name);
	Util::fprint("goal met : "+name);
	for (unsigned int n=0;n<mem->connAmt();n++)
	{
		Util::debug("transferring conn from "+mem->getConn(n)->getName());
		if (mem->getConn(n)->isPerc())
		{
			strive->addConn(mem->getConn(n),mem->getConnWeight(n));
		}
		else if (mem->getConn(n)->isDtrm())
		{
			addConn(mem->getConn(n),mem->getConnWeight(n));
		}
	}
	Util::debug("deleting mem");
	delete mem;
	mem=new Node();
	mem->setJob(Node::ShortTermMem);
	mem->setType(Node::Set);
}
void Goal::reset()
{
	Node::reset();
	meet->reset();
	strive->reset();
	//mem->reset();    //not sure if necessary
}
void Goal::unreset()
{
	Node::unreset();
	meet->unreset();
	strive->unreset();
}
void Goal::setName(string set)
{
	Node::setName("g_"+set);
	meet->setName(set+(string)"_meet");
	strive->setName(set+(string)"_strive");
	mem->setName(set+(string)"_mem");
}
void Goal::giveNewID()
{
	Node::giveNewID();
	meet->giveNewID();
	strive->giveNewID();
	mem->giveNewID();
}
void Goal::shun(Node* blasphemer)
{
	Node::shun(blasphemer);
	meet->shun(blasphemer);
	strive->shun(blasphemer);
	mem->shun(blasphemer);
}
Node* Goal::getMeet()
{
	return meet;
}
Node* Goal::getStrive()
{
	return strive;
}