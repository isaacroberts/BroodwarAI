#ifndef BASE_H
#define BASE_H

#include <vector>
#include "BWAPI.h"
#include "Ref.h"
#include "NetHandler.h"
using namespace BWAPI;

class Base
{
public:
	Base();
	Base(Unit*);
	void setPermInputs();
	void updateInputs();
	bool updateNet();//it returns whether or not to build an assm
	void check();
	Unit* getAssm() {return assm;}
	Unit* getBase() {return base;}
	std::vector<Unit*> getMinerals() {return mineral;}
	Unit* getMinerals(unsigned int ix);
	Unit* getRandMin();
	Net* getNet() {return net;}
private:
	Unit* base;
	std::vector<Unit*> mineral;
	Unit* assm;
	Net *net;
};

#endif