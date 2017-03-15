/* 
 * File:   Node.cpp
 * Author: Isaac
 * 
 * Created on April 8, 2012, 9:17 PM
 */
#include <iostream>
#include "Node.h"
#include "Goal.h"

using namespace std;

int Node::curNodeID=0;

Node::Node() 
{
    currentVal=0;
    thresh=.00254387;
    miscMod=-5000;
    state=1.0;
    lastState=0.0;
    checked=false;
    visited=false;
    name="newNode";
	id=curNodeID;
	curNodeID++;
	type=None;
	job=Unemployed;
	cond=NULL;
	condIsNot=false;
	netname="net";
	conns=vector<Node*>();
	connWeights=vector<double>();
}
double Node::get()//calculates and returns the state of the node
{
	if (checked)
    {
		return getState();
    }
    else if (visited) 
    {
	    return lastState;
    }
	else if (job==PermInput)
	{
		Util::fprint("MISTAKE! permInput not checked: "+name+" of "+netname);
		state=lastState;
		return getState();
	}
	else if (job==Input)
    {
		Util::fprint("MISTAKE! input not checked: "+name+" of "+netname);
		return getState();
    }
	else if (type==Set)
	{
		state=thresh;
		checked=true;
		return state;
	}
    else
    {
        visited=true;
		if (cond!=NULL)//if theres a conditional
		{
			if ((cond->get()>=.7) == condIsNot)//and if the conditional doesnt meet its condition
			{//										curval=0
	//			Util::debug(name+" cond not met");
				checked=true;
				if (subtractsThresh())
					currentVal=thresh;
				else currentVal=0;
				calcState(true);
				return getState();
			}
		}
        for (unsigned int n=0;n<conns.size();n++)
        {
			if (type==Maxer)
			{
				double val=connWeights[n]*conns[n]->get();
				if (val>currentVal)
					currentVal=val;
			}
			else
			{
				double got=0;
				if (conns[n]->getJob()==GoalDrive)
				{
					got=((Goal*)conns[n])->Goal::get();	
				}
				else 
				{
		            got=currentVal+=connWeights[n]*conns[n]->get();
				}
				Util::debug("    "+conns[n]->getName()+" -> "+name+" s:",got,false);
				Util::debug(" w:",connWeights[n]);
				currentVal+=connWeights[n]*got;
			}
        }
        checked=true;
        visited=false;
        calcState(true);
		Util::debug(name+" state= ",state);
		return getState();
    }
}
void Node::calcState(bool update)
{
	if (update)
		lastState=state;
	if (type==Bool)
        state= ((currentVal>=thresh)? 1: 0 );
	else if (type==Linear)
    {
        state= currentVal-thresh;
		if (miscMod!=-5000)
	        if (state>miscMod)
				state=miscMod;
    }
	else if (type==Flash)
    {
        if (currentVal>=thresh)
            state=1.0;
        else if (update)
        {
            if (state>miscMod)
			{
				if (miscMod==-5000)
				{
					miscMod=1;
					Util::fprint("warning! "+netname+":"+name+"(flash) needs a miscmod");
				}
	            state-=miscMod;//the higher miscMod is, the faster state nears 0	
			}
            else state=0;
        }
    }
	else if (type==OneShot)
	{
		if (update)
		{
			if (state>0)
			{
				if (miscMod==-5000)
				{
					Util::fprint("warning! "+name+"(oneshot) needs a miscmod");
					miscMod=.3;
				}
				state-=miscMod;
			}
			else if (state<=0)
			{
				if (currentVal>=thresh)
				{
					state=1.0;
				}
				else state=0;
			}
		}
	}
	else if (type==Cumltv)
	{
		state+=currentVal-thresh;
	}
	else if (type==Counter)
    {
       if (currentVal>=thresh)
	   {
		   state=0;
	   }
	   else
	   {
		   if (miscMod!=-5000)
			   state+=miscMod;
		   else
			   state++;
	   }
    }
	else if (type==Delay)
    {//when the threshhold is crossed, the state is set to .001*the amount of times the node should run Before
        //firing. each update the state is reduced by .001 until the state is .001, when the state is put to 1.0
        //if its not counting down and the curVal is less than thresh, state is 0.
        if (state<1&&state>0)
        {
            if (update)
            {
				if (miscMod==-5000)
				{
					Util::fprint("warning! "+name+"(delay) needs a miscmod");
					miscMod=.2;
				}
                if (state>miscMod)
                {
                    state+=miscMod;
                }
            }
        }
        else if (state==0)
        {
            if (currentVal>=thresh)
                state=miscMod;//miscMod is 1/(how many times the node should count down before firing)
            else
                state=0;
        }
		else if (state==1)
		{
			if (lastState==1)
				state=0;
		}
    }
	//else if (type==PermInput)
	else if (type==Multiply)
	{
		state=1;
		for (unsigned int n=0;n<conns.size();n++)
		{
			state*=(conns[n]->get()*connWeights[0]);
		}
		state-=thresh;
	}
	else if (type==Set)
	{
		state=thresh;
	}
	else if (type==Maxer)
	{
		state=currentVal-thresh;
	}
	else if (type==LinFlash)
	{
		if (currentVal-thresh>=state-miscMod)
		{
			state=currentVal-thresh;
		}
		else if (state-miscMod>=-thresh)
		{
			state-=miscMod;
		}
		else state=-thresh;
	}
	else if (type==Average)
	{//miscMod is the total weights
		if (miscMod==-5000)
		{
			miscMod=0;
			for (unsigned int n=0;n<connWeights.size();n++)
			{
				miscMod+=connWeights[n];
			}
		}
		if (miscMod==0)
			state=0;
		else
			state=currentVal/miscMod;
	}
}
double Node::getState() 
{//returns state without calculating
	if (type==Delay|| type==OneShot)
    {
        if (state<1)
            return 0;
		else return state;
    }
    else return state;     
}
void Node::shun(Node* blasphemer)
{
	for (unsigned int n=0;n<conns.size();n++)
	{
		if (conns[n]->getID()==blasphemer->getID())
		{
			conns.erase(conns.begin()+n);
			connWeights.erase(connWeights.begin()+n);
		}
	}
}
double Node::getLastState() {
    return lastState;
}
bool Node::justFired()
{
	if (isPerm())
		return false;
	else if (type==Bool)
        return (lastState==0&& state==1);
	else if (type==Delay)
        return (lastState!=1&& state==1);
	else if (type==OneShot)
		return (state==1);
	else if (job==Calc)
		return false;
	else if (type==PermInput)
		return false;
    else// if (type==Linear)
        return (state>lastState);
}
bool Node::justTurnedOff()
{
	if (type==Bool)
        return (lastState==1&&state==1);
	else if (type==Delay)
        return (lastState==1&&state!=1);
	else if (type==OneShot)
		return (lastState==1-miscMod);
	else if (job==Calc)
		return false;
	else if (isPerm())
		return false;
    else// if (type==Linear)
        return (state<lastState);
}
void Node::setCurVal(double set)
{
	Util::debug("set curval of "+name+":",set);
    currentVal=set;
    checked=true;
    lastState=state;	
	calcState(true);
}
void Node::addCurVal(double add)
{
	Util::debug("add curval of "+name+":",add);
    currentVal+=add;
    checked=true;
    calcState(false);
}
void Node::setState(double set)
{
	Util::debug("set state of "+name+":",set);
    lastState=state;
    state=set;
    checked=true;
    currentVal=thresh+set;
}
void Node::reset()
{
	if (!isPerm())
	{
	    currentVal=0;
	    lastState=state;
	    calcState(false);
		checked=false;
		visited=false;
	}
}
void Node::unreset()
{
	if (!isPerm())
	{
		state=lastState;
		checked=true;
		visited=true;
	}
}
void Node::setMiscMod(double set) {
    miscMod=set;
}
bool Node::hasMod() {

	if (miscMod==-5000) return false;//-5000 is the default miscMod
	
	if (type==Linear) return true;
	if (type==Delay) return true;
	if (type==Cumltv) return true;
	if (type==Flash) return true;
	if (type==OneShot) return true;
    return false;
}
double Node::getMiscMod(double) {
    return miscMod;
}
void Node::setThresh(double set) {
    thresh=set;
}
void Node::learnThresh(double add) {
    thresh+=add;
}
void Node::setConnWeight(Node* fromWhere,double set)
{
    bool any=false;
    for (unsigned int n=0;n<conns.size();n++)
    {
        if (conns[n]==fromWhere)
        {
            connWeights[n]=set;
            any=true;
        }
    }
    if (!any)
    {

        addConn(fromWhere,set);
    }
	if (type==Average)
	{
		miscMod=0;
		for (unsigned int n=0;n<connWeights.size();n++)
		{
			miscMod+=connWeights[n];
		}
	}
}
void Node::learnWeight(Node* fromWhere, double add)
{
    bool any=false;
    for (unsigned int n=0;n<conns.size();n++)
    {
        if (conns[n]==fromWhere)
        {
            connWeights[n]=add;
            any=true;
        }
    }
    if (!any)
    {
        addConn(fromWhere,add);
    }
	if (type==Average)
	{
		miscMod=0;
		for (unsigned int n=0;n<connWeights.size();n++)
		{
			miscMod+=connWeights[n];
		}
	}
}
void Node::addConn(Node* from,double weight)
{
    if (from->getID()==id)
        return;
    for (unsigned int n=0;n<conns.size();n++)
    {
        if (from->getID()==conns[n]->getID())
        {
            connWeights[n]+=weight;
            return;
        }
    }
    conns.push_back(from);
    connWeights.push_back(weight);
	if (type==Average)
	{
		miscMod+=weight;
	}
}
void Node::addConn(Node* from)
{
    if (from->getID()==id)
        return;
    for (unsigned int n=0;n<conns.size();n++)
    {
        if (from->getID()==conns[n]->getID())
            return;
    }
    conns.push_back(from);
    connWeights.push_back(1.0);
	if (type==Average)
	{
		miscMod+=1;
	}
}
void Node::setCondConn(Node* setCond)
{
	cond=setCond;
}
void Node::setCondConnIsNot(bool isNot)
{
	condIsNot=isNot;
}
double Node::getThresh() {
    return thresh;
}
vector<double> Node::getWeights() {
    return connWeights;
}
vector<Node*> Node::getConns() {
    return conns;
}
void Node::setJob(int set) {
	job=(Job)set;
}
void Node::setType(int set) {
	type=(NodeType)set;
	if (subtractsThresh())
		if (thresh==.00254387)
		{
			thresh=0;
		}
}
int Node::getJob() 
{
    return job;
}
int Node::getType()
{
    return type;
}
bool Node::subtractsThresh()
{
	if (type==Linear) return true;
	if (type==LinFlash) return true;
	if (type==Cumltv) return true;
	if (type==Divide) return true;
	if (type==Multiply) return true;
	if (type==Invert) return true;
	if (type==Set) return true;
	return false;
}
bool Node::comparesToThresh()
{
	if (type==Bool) return true;
	if (type==Flash) return true;
	if (type==OneShot) return true;
	if (type==Delay) return true;
	if (type==Counter) return true;
	return false;
}
bool Node::isDtrm() {
	if (job==Determ) return true;
	if (job==Output) return true;
	if (job==GoalDrive) return true;
	if (job==GoalStrive) return true;
	return false;
}
bool Node::isPerc() {
	if (job==Input) return true;
	if (job==Percp) return true;
	if (job==PermInput) return true;
	if (job==PermPercp) return true;
	if (job==ConstPercp) return true;
	if (job==Expect) return true;
	if (job==GoalMeet) return true;
	return false;
}
bool Node::isPerm()
{
	if (job==PermInput) return true;
	if (job==PermPercp) return true;
	return false;
}
bool Node::isEditable()
{
	if (job==Input) return false;
	else if (job==Memory) return false;
	else if (job==ConstPercp) return false;
	else if (job==Calc) return false;
	else if (isPerm()) return false;
    else return true;
}
void Node::setName(string set) {//name will be used for the computer to determine the use of the node
    name=set;
}
string Node::getName() {
    return name;
}
Node::Node(const Node& orig) 
{
}

Node::~Node() 
{

}

