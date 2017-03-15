#include <algorithm>
#include "Area.h"

Area*** Area::area;
std::list<Area*> Area::checked=std::list<Area*>();
int Area::curID=0;
int Area::AREA_SIZE=300;
int Area::cols=0,Area::rows=0;
Area::Area()
{
	net=NetHandler::initNet("area.txt",Net::Area);
	occupant=std::vector<Unit*>();
	nrstUnitDist=9999999999;
	nrstUnit=NULL;
	expediency=0;
	diffuseExpd=0;
	id=curID;
	curID++;
}
Area::Area(int gridX,int gridY)
{
	gx=gridX;
	gy=gridY;
	left=gridX*AREA_SIZE;
	top=gridY*AREA_SIZE;
	calcEdges();
	net=NetHandler::initNet("area.txt",Net::Area);
	occupant=std::vector<Unit*>();
	nrstUnitDist=9999999999;
	nrstUnit=NULL;
	id=curID;
	curID++;
	expediency=0;
	diffuseExpd=0;
}
void Area::setPos(int x,int y)
{
	left=x;
	top=y;
	calcEdges();
}
void Area::calcEdges()
{
	right=left+AREA_SIZE;
	if (right>Broodwar->mapWidth()*32)
		right=Broodwar->mapWidth()*32+1;
	bottom=top+AREA_SIZE;
	if (bottom>Broodwar->mapHeight()*32)
		bottom=Broodwar->mapHeight()*32+1;
	center=Position(left+AREA_SIZE/2,top+AREA_SIZE/2);
}
bool Area::build(UnitType* what,Unit* worker)
{
	return build(what,worker,true);
}
bool Area::build(UnitType* what,Unit* worker,bool firstIt)
{
	if (firstIt)
		checked.clear();
	for (int y=top;y<bottom;y+=32*what->tileHeight())
	{
		for (int x=left;x<right;x+=32)
		{
			BWAPI::TilePosition tile=BWAPI::TilePosition(x/32,y/32);
			Broodwar->drawBoxMap(x,y,x+5,y+5,Color(Colors::Blue),true);
			
			if (Broodwar->canBuildHere(worker,tile,*what,true))
			{
				if (worker->build(tile,*what))
				{
					checked.clear(); //in case build(what,worker) is using recursive
					return true;
				}
			}
		}
	}
	//MORE MAGIC
	checked.push_back(this);
	std::vector<Area*> naybor=getNeighbors();
	for each (Area* nb in naybor)
	{
		if (!inChecked(nb))
		{
			Util::debug("Area:build going to region ",nb->getGX(),nb->getGY());
			if (nb->someIsWalkable())
				if (nb->getExpediency()>50)//todo: find a better value
				{
					if ((nb)->build(what,worker,false))
						return true;
				}
		}
	}
	
	return false;
}
void Area::draw()
{
	//draw everything
	Broodwar->drawBoxMap(left,top,right,bottom, BWAPI::Color((int)(expediency/1000)),false);
	Broodwar->drawBoxMap(center.x()-2,center.y()-2,center.x()+2,center.y()+2,BWAPI::Color(int(expediency/1000)),true);
	//Broodwar->drawTextMap(center.x(),center.y(),"%i,%i",gx,gy);
	Broodwar->drawTextMap(center.x(),center.y(),"%.0f",expediency);
}
bool Area::unitLeftArea(Unit* who)
{
	if (!who->exists()) return true;
	else if (!who->isVisible() )//|| Broodwar->isFlagEnabled(Flag::CompleteMapInformation))
		return true;
	else if (!contains(who->getPosition())) return true;
	return false;
}
void Area::update()
{
	if (occupant.size()>0)
	{
		for (unsigned int n=0;n<occupant.size();n++)
		{
			if (!contains(occupant[n]->getPosition()))
			{
				Area::getArea(occupant[n]->getPosition())->addOccupant(occupant[n]);
			}
		}
		occupant.erase( //erase the occupants that have left:
			std::remove_if(occupant.begin(),occupant.end(),//give it the range
			bind1st(mem_fun(&Area::unitLeftArea),this)),//take a roundabout way to give it Area::unitLeftArea(...,this) which tells remove_if who should be erased
			occupant.end());//remove_if puts all of the elements to be erased at the end
	}
	netUpdate();
	expediency*=10;
	Util::debug("",gx,false);
	Util::debug(",",gy,false);
	Util::debug("  exped = ",expediency);
	checked.clear();
	checked.push_back(this);
	diffuse();
}
void Area::setPermInputs()
{
	for (unsigned int n=0;n<net->inputAmt();n++)
	{
		if (net->getInput(n)->getName()=="hasRes")
		{
			std::set<Unit*> whosHere=Broodwar->getUnitsInRectangle(left,top,right,bottom);
			for each(Unit* u in whosHere)
			{
				if (u->getType().isMineralField() || u->getType().getID()==UnitTypes::Resource_Vespene_Geyser.getID())
				{
					net->getInput(n)->setCurVal(1);
					break;
				}
			}
		}
	}
}
void Area::updateInputs()
{
	Util::fprint("area updating inputs");
	double enemyDangerVal=0,enemyKillVal=0;
	double ownDangerVal=0,ownKillVal=0;
	bool ownBldgsHere=false,enBldgsHere=false,enBase=false,ownUnitHere=false;
	for (unsigned int n=0;n<occupant.size();n++)
	{
		if (occupant[n]->getPlayer()->isEnemy(Broodwar->self()))
		{
			enemyDangerVal+=Ref::getDangerVal(&occupant[n]->getType());
			enemyKillVal+=Ref::getKillVal(&occupant[n]->getType());
			if (!occupant[n]->getType().canMove())
			{
				enBldgsHere=true;
				if (occupant[n]->getType().isResourceDepot())
					enBase=true;
			}
		}
		else if (occupant[n]->getPlayer()->isAlly(Broodwar->self()) || occupant[n]->getPlayer()->getID()==Broodwar->self()->getID())
		{
			ownDangerVal+=Ref::getDangerVal(&occupant[n]->getType());
			ownKillVal+=Ref::getKillVal(&occupant[n]->getType());
			if (!occupant[n]->getType().canMove())
				ownBldgsHere=true;
			else ownUnitHere=true;
		}
	}
	for (unsigned int n=0;n<net->inputAmt();n++)
	{
		if (net->getInput(n)->getName()=="visible")
		{
			net->getInput(n)->setCurVal(Broodwar->isVisible(Util::posToTile(center)));
		}
		else if (net->getInput(n)->getName()=="ownDangerVal")
		{
			net->getInput(n)->setCurVal(ownDangerVal);
		}
		else if (net->getInput(n)->getName()=="ownKillVal")
		{
			net->getInput(n)->setCurVal(ownKillVal);
		}
		else if (net->getInput(n)->getName()=="enDangerVal")
		{
			net->getInput(n)->setCurVal(enemyDangerVal);
		}
		else if (net->getInput(n)->getName()=="enKillVal")
		{
			net->getInput(n)->setCurVal(enemyKillVal);
		}
		else if (net->getInput(n)->getName()=="ownBldgsHere")
		{
			net->getInput(n)->setCurVal(ownBldgsHere);
		}
		else if (net->getInput(n)->getName()=="noEnBldgsHere")
		{
			net->getInput(n)->setCurVal(!enBldgsHere);
		}
		else if (net->getInput(n)->getName()=="enHasBase")
		{
			net->getInput(n)->setCurVal(enBase);
		}
		else if (net->getInput(n)->getName()=="distFromNrstUnit")
		{
			if (ownUnitHere)
			{
				net->getInput(n)->setCurVal(0);
			}
			else if (Ref::hasSoldiers)
			{
				findNrstUnit();
				net->getInput(n)->setCurVal(nrstUnitDist);
			}
			else net->getInput(n)->setCurVal(9999999999);
		}
	}
}
void Area::netUpdate()
{
	for (unsigned int n=0;n<net->outputAmt();n++)
	{
//		Util::fprint("area output=" +net->getOutput(n)->getName());
		if (net->getOutput(n)->getName()=="expediency")
		{
			expediency=net->getOutput(n)->get();
		}
	}
	//Util::fprint("} area->net");
}
void Area::diffuse()
{
	if (expediency>=1000)
	{
		std::vector<Area*> neybor=getNeighbors();
		for each (Area* nb in neybor)
		{
			if (!inChecked(nb))
			{
				checked.push_back(nb);
				nb->diffuse(expediency-1000.0);
			}
		}
	}
}
void Area::diffuse(double expd)
{	
//	Util::debug("  diffusing ",expd,false);
//	Util::debug(" to ",gx,gy);
	diffuseExpd+=expd;
	if (expd>=1000)
	{
		std::vector<Area*> neybor=getNeighbors();
		for each (Area* nb in neybor)
		{
			if (!inChecked(nb))
			{
				checked.push_back(nb);
				nb->diffuse(expd-1000.0);
			}
		}
	}
}
void Area::addInDiffuseExpd()
{
	Util::debug("d:"+Util::makeLength(Util::numToString(diffuseExpd),6),false);
	expediency+=diffuseExpd;
}
std::vector<Area*> Area::getNeighbors()
{
	std::vector<Area*> nabor=std::vector<Area*>();
	if (gx!=0)
		nabor.push_back(Area::getArea(gx-1,gy));
	if (gx!=cols-1)
		nabor.push_back(Area::getArea(gx+1,gy));
	if (gy!=0)
		nabor.push_back(Area::getArea(gx,gy-1));
	if (gy!=rows-1)
		nabor.push_back(Area::getArea(gx,gy+1));
	return nabor;
}
void Area::addOccupant(Unit* add)
{
	occupant.push_back(add);
}
bool Area::contains(Position pos)
{
	if (pos.x()>=left && pos.x()< right)
	{
		if (pos.y()>=top && pos.y() < bottom)
			return true;
		else return false;
	}
	else return false;
}
bool Area::allIsWalkable()
{
	for (int x=left;x<right;x++)
	{
		for (int y=top;y<bottom;y++)
		{
			if (!Broodwar->isWalkable(x/8,y/8))
				return false;
		}
	}
	return true;
}
bool Area::someIsWalkable()
{
	for (int x=left;x<right;x++)
	{
		for (int y=top;y<bottom;y++)
		{
			if (Broodwar->isWalkable(x/8,y/8))
				return true;
		}
	}
	return false;
}
void Area::findNrstUnit()
{
	nrstUnit=NULL;
	nrstUnitDist=9999999999999;
	for (int rad=100;nrstUnit==NULL;rad=rad*2)
	{
		if (rad>Broodwar->mapWidth()+Broodwar->mapHeight())
			break;
		std::set<Unit*> inradius=Broodwar->getUnitsInRadius(center,rad);
		for each (Unit* it in inradius)
		{
			if ((it)->getPlayer()->getID()==Broodwar->self()->getID())
			{
				if ((it)->getType().canMove())
				{
					double thisDist=Util::sqrdDistance((it)->getPosition(),center);
					if (thisDist<nrstUnitDist)
					{
						nrstUnit=(it);
						nrstUnitDist=thisDist;
					}
				}
			}
		}
	}
	if (nrstUnit==NULL)
	{
		Util::fprint("area::findNet(): there is no unit anywhere on the map.");
		nrstUnit=*(Broodwar->self()->getUnits().begin());
	}
	nrstUnitDist=sqrt(nrstUnitDist);
}
Area::~Area()
{
	NetHandler::excommunicate(net);
}

void Area::init() //static
{
	cols=(int)ceil(((double)Broodwar->mapWidth()*32.0)/(double)AREA_SIZE);//mapWidth() and height return buildtiles
	rows=(int)ceil(((double)Broodwar->mapHeight()*32.0)/(double)AREA_SIZE);
	area=new Area**[cols];
	for (int x=0;x<cols;x++)
	{
		area[x]=new Area*[rows];
		for (int y=0;y<rows;y++)
		{
			area[x][y]=new Area(x,y);
		}
	}
	Util::fprint("",cols,false);
	Util::fprint(" by ",rows,false);
	Util::fprint(" areas created");
	Util::fprint("Map size: ",Broodwar->mapWidth()*32,false);
	Util::fprint(", ",Broodwar->mapHeight()*32);
}
void Area::setAllPermInputs()
{
	for (int x=0;x<cols;x++)
	{
		for (int y=0;y<rows;y++)
		{
			area[x][y]->setPermInputs();
		}
	}
}
void Area::updateAllInputs()//static
{
	Util::debug("area inputs");
	for (int x=0;x<cols;x++)
	{
		for (int y=0;y<rows;y++)
		{
			area[x][y]->updateInputs();
			area[x][y]->resetDiffuseExpd();
		}
	}
}
void Area::updateAll()//static
{
	Util::debug("<area>");
	for (int x=0;x<cols;x++)
	{
		for (int y=0;y<rows;y++)
		{
		//	if ((x+y+Ref::updates())%5==0)
			{
				area[x][y]->update();	
			}
		//	else area[x][y]->getNet()->unreset();
		}
	}
	for (int y=0;y<rows;y++)
	{
		for (int x=0;x<cols;x++)
		{
			Util::debug("e:"+Util::makeLength(Util::numToString(area[x][y]->getExpediency()),6),false);
			Util::debug("|",false);
		}
		Util::debug();
		for (int x=0;x<cols;x++)	
		{
			area[x][y]->addInDiffuseExpd();
			Util::debug("|",false);
			area[x][y]->draw();
		}
		Util::debug();
		Util::debug("----------------------------------------------------------------------");
	}
	Util::debug("</area>");
}
void Area::giveOccupant(Unit* add)//static
{
	Area::getArea(add->getPosition())->addOccupant(add);
}
Area* Area::getArea(int n)
{
	return area[n/cols][n%cols];
}
Area* Area::getArea(BWAPI::Position pos) //static
{
	for (int x=0;x<cols;x++)
	{
		for (int y=0;y<rows;y++)
		{
			if (area[x][y]->contains(pos))
			{
				return area[x][y];
			}
		}
	}
	Util::fprint("error! Area::getArea(Position): someones looking for a position not contained in anything",false);
	Util::fprint("  pos x=",pos.x(),false);
	Util::fprint("  y=",pos.y());
	return new Area();
}
bool Area::inChecked(Area* reg)//static
{
	for each (Area* it in checked)
	{
		if ((it)->getID()==reg->getID())
			return true;
	}
	return false;
}

