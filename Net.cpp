/* 
 * File:   Net.cpp
 * Author: Isaac
 * 
 * Created on April 8, 2012, 9:40 PM
 */
#include <iostream>
#include <fstream>
#include <math.h>

#include "Net.h"

using namespace std;

int Net::curNetID=0;

Net::Net() 
{
    node=vector<Node*>();
    input=vector<Node*>();
	output=vector<Node*>();
	type=None;
	id=curNetID;
	curNetID++;
	name="net";
	isTrim=false;
}
Net::Net(int setType)
{
	node=vector<Node*>();
    input=vector<Node*>();
	output=vector<Node*>();
	type=None;
	id=curNetID;
	curNetID++;
	type=(NetType)setType;
	name="net";
	isTrim=false;
}
void Net::createNodes(int amt)
{
	for (int n=0;n<amt;n++)

	{
		node.push_back(new Node());
		node[node.size()-1]->setNetname(name);
	}
}
void Net::addNode(Node* add)
{
	add->setNetname(name);
	node.push_back(add);
}
void Net::initialize()
{
	for (unsigned int n=0;n<node.size();n++)
    {
		if (node[n]->getJob()==Node::Input || node[n]->getJob()==Node::PermInput)
            input.push_back(node[n]);
		else if (node[n]->getJob()==Node::Output)
		{
	        output.push_back(node[n]);
		}
    }
	
}
void Net::resetNodes()
{
	for (unsigned int n=0;n<node.size();n++)
	{
		node[n]->reset();
	}
}
void Net::unreset()
{
	for (unsigned int n=0;n<node.size();n++)
	{
		node[n]->unreset();
	}
}
void Net::write()
{
    ofstream mem;
    mem.open("netData.txt");
	/*
    mem<<"#"<<node.size()<<"{";
    for (unsigned int n=0;n<node.size();n++)
    {
        mem<<"/"<<n<<"=\""<<node[n]->getName()<<"\"-"<<node[n]->getThresh()<<"(";
        vector<double> weights=node[n]->getWeights();
        vector<Node*>* to=node[n]->getConns();
        for (unsigned int c=0;c<to->size();c++)
        {
            int id=-1;
            for (unsigned int q=0;q<node.size();q++)
            {
                if (node[q]==to->at(c))
                {
                    id=q;
                    break;
                }
            }
            mem<<"<"<<id<<"|"<<weights[c];
        }
        mem<<")";
    }
    mem<<"}";
	*/
	mem<<"this is now a file";
    mem.close();
    
}
void Net::giveNewID()
{
	id=curNetID;
	curNetID++;
	for (unsigned int n=0;n<node.size();n++)
	{
		node[n]->giveNewID();
	}
}
void Net::excommunicate(Node* blasphemer)
{
	for (unsigned int n=0;n<node.size();n++)
	{
		node[n]->shun(blasphemer);
	}
}
bool Net::canConnect(Node* from,Node* to)
{
    if (!to->isEditable())
        return false;
    else if (to->isDtrm())
        return true;
    else if (from->isPerc())
        return true;
    else 
        return false;
}
Node* Net::getNode(unsigned int id)
{
	if (id<node.size())
		return node[id];
	else return new Node();
}
Node* Net::getNode(std::string name)
{
	for (unsigned int n=0;n<node.size();n++)
	{
		if (node[n]->getName()==name)
			return node[n];
	}
	return new Node();
}
void Net::replace(int id,Node* replaceWith)
{
	node[id]=replaceWith;
}
Node* Net::getInput(unsigned int ix) {
	return input[ix];
}
Node* Net::getOutput(unsigned int ix) {
	return output[ix];
}
Net::NetType Net::stringToNetType(string t)
{
	if (t=="econ")
		return Econ;
	else if (t=="army")
		return Army;
	else if (t=="area")
		return Area;
	else if (t=="goal")
		return Goal;
	else if (t=="base")
		return Base;
	else if (t=="unittype")
		return UnitType;
	else if (t=="cmndgrp")
		return CmndGroup;
	else return None;
}
Net::Net(const Net& orig) {
}

Net::~Net()
{
	for (unsigned int n=0;n<node.size();n++)
	{
		delete node[n];
	}
	BWAPI::Broodwar->printf("Net %s:",name);
	BWAPI::Broodwar->printf("OH GOD NO PLEASE ILL BE A GOOD NET PLEASE DONT DELETE ME");
}
