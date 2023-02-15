#include "pch.h"
#include "Engine.h"
#include "PhyUnit.h"

VEngine::VEngineCore* VEngine::VEngineCore::init()
{
	//Globalizza le unità fondamentali
	//Altrimenti il programma non funziona
	Phy::UnitComponentFactory::getInstance()->globalizeUnit(&(Phy::UnitComponentFactory::AMPERE));
	Phy::UnitComponentFactory::getInstance()->globalizeUnit(&(Phy::UnitComponentFactory::CANDELA));
	Phy::UnitComponentFactory::getInstance()->globalizeUnit(&(Phy::UnitComponentFactory::GRAMMO));
	Phy::UnitComponentFactory::getInstance()->globalizeUnit(&(Phy::UnitComponentFactory::KELVIN));
	Phy::UnitComponentFactory::getInstance()->globalizeUnit(&(Phy::UnitComponentFactory::METRO));
	Phy::UnitComponentFactory::getInstance()->globalizeUnit(&(Phy::UnitComponentFactory::MOLE));
	Phy::UnitComponentFactory::getInstance()->globalizeUnit(&(Phy::UnitComponentFactory::SECONDO));
	Phy::UnitComponentFactory::getInstance()->globalizeUnit(&(Phy::UnitComponentFactory::_1));
	return this;
}
