#include "../idlib/precompiled.h"
#pragma hdrstop

#include "RoundManager.h"

//8820 -8343 128

void RoundManager::Init() 
{
	round = 1;
	roundEnded = true;
	roundStarted = false;
}

void RoundManager::StartRound() 
{
	roundStarted = true;

	for (int i = 0; i < playerUnits.Num(); i++) 
	{
		idEntity* playerUnit = gameLocal.entities[playerUnits[i]];
		playerUnit->fl.notarget = false;
	}

	for (int i = 0; i < enemyUnits.Num(); i++)
	{
		idEntity* enemyUnit = gameLocal.entities[enemyUnits[i]];
		enemyUnit->fl.notarget = false;
	}
}

bool RoundManager::CheckEndRound() 
{
	if (roundEnded) 
	{
		return roundEnded;
	}

	return playerUnits.Num() == 0 || enemyUnits.Num() == 0;
}

void RoundManager::SpawnEnemyUnits() 
{
	const char* units[] = {
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

	idDict savedSpawnPositions;

	idDict dict;
	idVec3 origin;
	float yaw = 177;

	bool validPositionSelected = false;

	for (int i = 0; i < round; i++) 
	{
		int monster_idx = gameLocal.random.RandomInt(round / 4);

		dict.Set("classname", va("%s", units[monster_idx]));
		dict.Set("angle", va("%f", yaw));

		while (!validPositionSelected) 
		{
			origin = CENTER_SPAWN_POINT + idVec3(10 + (10 * round / 2.0 * gameLocal.random.CRandomFloat()), 10 + (10 * round / 2.0 * gameLocal.random.CRandomFloat()), 0);
			savedSpawnPositions.GetBool(va("%f%f%f", origin.x, origin.y, origin.z), "1", validPositionSelected);
		}

		dict.Set("origin", origin.ToString());

		dict.SetFloat("visRange", 2048);
		dict.SetFloat("earRange", 2048);
		dict.SetFloat("awareRange", 2048);

		dict.SetInt("notarget", 1);

		idEntity* newEnt = NULL;
		gameLocal.SpawnEntityDef(dict, &newEnt);

		if (newEnt) 
		{
			gameLocal.Printf("Spawned Enemy Entity '%s'\n", newEnt->name.c_str());
			enemyUnits.Append(newEnt->entityNumber);
			savedSpawnPositions.SetBool(va("%f%f%f", origin.x, origin.y, origin.z), false);
		}
	}
}

void RoundManager::RemoveEnemyUnit(int entityNum) 
{
	enemyUnits.Remove(entityNum);
}

void RoundManager::AddPlayerUnit(int entityNum) 
{
	playerUnits.Append(entityNum);
}

void RoundManager::RemovePlayerUnit(int entityNum) 
{
	playerUnits.Remove(entityNum);
}

void RoundManager::Think() 
{
	if (roundEnded) 
	{
		round = round > 1 ? round + 1 : round;
		idPlayer* player = gameLocal.GetLocalPlayer();

		if (!player) 
		{
			return;
		}

		if (roundStarted && playerUnits.Num() == 0)
		{
			round = 1;
			player->Kill(false, false);

			return;
		}

		player->hud->SetStateString("rtext", va("Round %d", gameLocal.roundManager.round));
		player->hud->HandleNamedEvent("showRound");

		roundEnded = false;
		
		if (enemyUnits.Num() == 0) 
		{
			SpawnEnemyUnits();
		}

		return;
	}

	roundEnded = CheckEndRound() && roundStarted;
}