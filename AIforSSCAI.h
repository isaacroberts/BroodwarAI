#pragma once

#include <BWAPI.h>
#include <BWTA.h>
#include <windows.h>
#include <BWAPI/Unit.h>
#include <BWAPI/AIModule.h>
#include <string>

#include <ctime>

#include "Util.h"
#include "Ref.h"
#include "Area.h"
#include "Economy.h"
#include "Army.h"
#include "NetHandler.h"

extern bool analyzed;
extern bool analysis_just_finished;
extern BWTA::Region* home;
extern BWTA::Region* enemy_base;
DWORD WINAPI AnalyzeThread();

using namespace BWAPI;

class AIforSSCAI : public BWAPI::AIModule
{
public:
	AIforSSCAI(){}
	virtual void onFrame();
	virtual void onStart();
	virtual void onSendText(string);
	virtual void onUnitCreate(Unit*);
	virtual void onUnitComplete(Unit*);
	virtual void onUnitDiscover(Unit*);
	virtual void onUnitDestroy(Unit*);
	virtual void onUnitMorph(Unit*);
	virtual void onEnd(bool win);
	void goalnetUpdateInputs();
	
private:
	Economy *econ;
	Army *army;
	Net *goalnet;
};
