#pragma once
#ifndef PHY_UNIT_API
#define PHY_UNIT_API
#include "PhyUnit.h"

#define PHYAPI_INIT() (new VEngine::VEngineCore())->init();

/*
Definizione delle UnitComponent (o UnitType)
*/
#define PHYAPI_AMPERE_UT Phy::UnitComponentFactory::AMPERE
#define PHYAPI_CANDELA_UT Phy::UnitComponentFactory::CANDELA
#define PHYAPI_GRAMMO_UT Phy::UnitComponentFactory::GRAMMO
#define PHYAPI_KELVIN_UT Phy::UnitComponentFactory::KELVIN
#define PHYAPI_METRO_UT Phy::UnitComponentFactory::METRO
#define PHYAPI_MOLE_UT Phy::UnitComponentFactory::MOLE
#define PHYAPI_SECONDO_UT Phy::UnitComponentFactory::SECONDO
#define PHYAPI_1_UT Phy::UnitComponentFactory::_1

/*
Definizione delle Unit
*/
#define PHYAPI_AMPERE Phy::UnitFactory::AMPERE
#define PHYAPI_CANDELA Phy::UnitFactory::CANDELA
#define PHYAPI_GRAMMO Phy::UnitFactory::GRAMMO
#define PHYAPI_KELVIN Phy::UnitFactory::KELVIN
#define PHYAPI_METRO Phy::UnitFactory::METRO
#define PHYAPI_MOLE Phy::UnitFactory::MOLE
#define PHYAPI_SECONDO Phy::UnitFactory::SECONDO
#define PHYAPI_1 Phy::UnitFactory::_1

#define PHYAPI_UNIT_FACTORY Phy::UnitComponentFactory::getInstance()
#define PHYAPI_CREATE_GLOBAL(Name,Symbol) Phy::UnitComponentFactory::getInstance()->createUnit((const char*)Name,(const char*) Symbol)
#define NORMALIZE_UNIT(Unit) ((Phy::UnitComponent const*)Unit)->normalize()

#endif // !PHY_UNIT_API