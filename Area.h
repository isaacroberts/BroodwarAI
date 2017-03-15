#pragma once

#include <vector>
#include <BWAPI.h>
#include "Ref.h"
#include "Util.h"
#include "NetHandler.h"

using namespace BWAPI;

class Area
{
public:
	Area();
	Area(int x,int y);
	void setPos(int x,int y);
	void calcEdges();
	void draw();
	bool unitLeftArea(Unit*);
	static bool unitLeftArea(Unit* u,Area* ar){return ar->unitLeftArea(u);}
	void update();
	void setPermInputs();
	void updateInputs();
	void netUpdate();
	bool build(UnitType*,Unit* worker);
	bool build(UnitType*,Unit* worker,bool firstIt);
	void addOccupant(Unit*);
	void findNrstUnit();
	Net* getNet() {return net;}
	double getExpediency() {return expediency;}
	void addExpediency(double add) {expediency +=add;}
	bool allIsWalkable();
	bool someIsWalkable();
	bool contains(Position);
	void diffuse();//initial call
	void diffuse(double exped);
	void resetDiffuseExpd() {diffuseExpd=0;}
	void addInDiffuseExpd() ;
	std::vector<Area*> getNeighbors();
	int getID() {return id;}
	Position getCenter() {return center;}
	int getGX() {return gx;}
	int getGY() {return gy;}
	~Area(void);
	static void init();
	//void volunteerNeighbors(int areaIx);
	static void setAllPermInputs();
	static void updateAllInputs();
	static void updateAll();
	static void giveOccupant(Unit*);
	static bool inChecked(Area*);
	static Area* getArea(int x,int y){return area[x][y];}
	static Area* getArea(Position);
	static Area* getArea(int n);
private:
	double expediency;
	double diffuseExpd;
	Net* net;
	std::vector<Unit*> occupant;
	static std::list<Area*> checked;
	static Area ***area;
	double nrstUnitDist;
	Unit* nrstUnit;
	int left,right,top,bottom;
	int gx,gy;
	Position center;
	int id;
	static int curID;
	static int cols,rows;
	static int AREA_SIZE;
};
