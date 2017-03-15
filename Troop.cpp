#include "Troop.h"

Troop::Troop()
{
}
Troop::Troop(Unit* self)
{
	unit=self;
}
void Troop::autonomous()
{
	Util::debug("troop "+unit->getType().getName()+" moving autonomous");
	std::set<Unit*> nearby=Broodwar->getUnitsInRadius(unit->getPosition(),Util::getWeaponRange(unit->getType()));
	Unit* attack=NULL;
	if (nearby.begin()!=nearby.end())
	{
		double mostKillExpd=0;
		for (std::set<Unit*>::iterator enm=nearby.begin();enm!=nearby.end();enm++)
		{
			if ((*enm)->getPlayer()->isEnemy(Broodwar->self()))
			{
				WeaponType* untWeapon=&((*enm)->getType().isFlyer()? unit->getType().airWeapon() : unit->getType().groundWeapon());
				//killExp= ((shots require to kill)+killVal+dangerVal)/health   -distance
				double killExpd=(untWeapon->damageAmount() - (*enm)->getType().armor());
				killExpd+=Ref::getClass(&(*enm)->getType())->getKillVal()*.005;//add coeffiecents
				killExpd+=Ref::getClass(&(*enm)->getType())->getDangerVal()*.006;
				killExpd/=(*enm)->getHitPoints();
				killExpd-=Util::distance(unit->getPosition(),(*enm)->getPosition());
				if (killExpd>mostKillExpd)
				{
					mostKillExpd=killExpd;
					attack=*enm;
				}
			}
		}
		if (attack!=NULL)
		{
			Util::debug("attacking "+attack->getType().getName());
			unit->attack(attack,false);
		}
	}
	if (attack==NULL)
	{
		Area* curArea=Area::getArea(unit->getPosition());
		double highestNaborExpd=curArea->getExpediency();
		Area* expdestNabor=NULL;
		std::vector<Area*> nabors=curArea->getNeighbors();
		for each (Area* nb in nabors)
		{
			if (unit->hasPath(nb->getCenter()))
			{
				double curNaborExpd=nb->getExpediency();
				if (curNaborExpd>highestNaborExpd)
				{
					highestNaborExpd=curNaborExpd;
					expdestNabor=nb;
				}
			}
		}
		if (expdestNabor!=NULL)
		{
			if (expdestNabor->allIsWalkable())
			{
				Util::debug("moving to area ",expdestNabor->getGX(),expdestNabor->getGY());
				unit->move(expdestNabor->getCenter());
			}
		}
	}
}
Troop::~Troop()
{
}
