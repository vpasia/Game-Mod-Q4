#ifndef __PLAYERUNITINVENTORY_H_
#define __PLAYERUNITINVENTORY_H_



#define PURCHASABLE_MASK 0x3F

class PlayerUnitInventory
{
private:
	int cash;

	const char* UNITS[10] = {
					"monster_slimy_transfer",
					"monster_strogg_marine_mgun",
					"monster_grunt",
					"monster_scientist",
					"monster_convoy_ground",
					"monster_gunner",
					"monster_berserker",
					"monster_iron_maiden",
					"monster_gladiator",
					"monster_stream_protector"
	};

	const unsigned int UNIT_COSTS[6] = {
		150,
		150,
		250,
		400,
		400,
		500
	};


public:
	unsigned int availableUnits;
	int purchasedCount;

	const char* GetSpawnUnit(int index);

	int GetCash() { return cash; }
	void GiveCash(unsigned int dcash) { cash += dcash; }
	void SetCash(unsigned int dcash) { cash = dcash; }
	bool AttemptToBuyUnit(int index);


	PlayerUnitInventory()
	{
		cash = 0;
		availableUnits = 0x3C0;
		purchasedCount = 0;
	}
};

#endif