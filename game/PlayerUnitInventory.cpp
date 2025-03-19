#include "../idlib/precompiled.h"
#pragma hdrstop

#include "PlayerUnitInventory.h"

/*
===============
PlayerUnitInventory::AttemptToBuyUnit
===============
*/
bool PlayerUnitInventory::AttemptToBuyUnit(int index)
{
	int cost = UNIT_COSTS[index - 1];

	if (cash - cost < 0)
	{
		return false;
	}

	cash -= cost;
	availableUnits |= 1 << (6 - index);
	return true;
}

/*
===============
PlayerUnitInventory::GetSpawnUnit
===============
*/
const char* PlayerUnitInventory::GetSpawnUnit(int index)
{
	if ((availableUnits >> (10 - index - 1)) & 1)
	{
		return UNITS[index];
	}
	return NULL;
}