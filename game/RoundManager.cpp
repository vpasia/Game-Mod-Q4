#include "../idlib/precompiled.h"
#pragma hdrstop

#include "RoundManager.h"

#define MIN_SPAWN_DISTANCE 50

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
		if(playerUnit) playerUnit->fl.notarget = false;
	}

	for (int i = 0; i < enemyUnits.Num(); i++)
	{
		idEntity* enemyUnit = gameLocal.entities[enemyUnits[i]];
		if(enemyUnit) enemyUnit->fl.notarget = false;
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

	idRandom2 rng(gameLocal.random.RandomInt());

	idList<idVec3> savedSpawnPositions;

	idDict dict;
	idVec3 origin;
	float yaw = -58;

	bool validPositionSelected = false;

	for (int i = 0; i < round; i++) 
	{
		int monster_idx = rng.RandomInt(round / 4);

		dict.Set("classname", va("%s", units[monster_idx]));
		dict.Set("angle", va("%f", yaw));

		while (!validPositionSelected) 
		{
			float randomOffsetX = rng.CRandomFloat();
			float randomOffsetY = rng.CRandomFloat();

			int xRight = randomOffsetX > 0 ? 1 : -1;
			int yRight = randomOffsetY > 0 ? 1 : -1;

			float range = ((100 + (10 * round)) - MIN_SPAWN_DISTANCE) + 1;

			idVec3 offset(xRight * ((abs(randomOffsetX) * range) + MIN_SPAWN_DISTANCE), yRight * ((abs(randomOffsetY) * range) + MIN_SPAWN_DISTANCE), 0);
			origin = CENTER_SPAWN_POINT + offset;

			validPositionSelected = true;

			for (int j = 0; j < savedSpawnPositions.Num(); j++) 
			{
				idVec3 position = savedSpawnPositions[j];

				if (abs(origin.x - position.x) < MIN_SPAWN_DISTANCE && abs(origin.y - position.y) < MIN_SPAWN_DISTANCE) 
				{
					validPositionSelected = false;
					break;
				}
			}
		}

		dict.Set("origin", origin.ToString());

		dict.SetFloat("visRange", 9000);
		dict.SetFloat("earRange", 9000);
		dict.SetFloat("awareRange", 9000);

		dict.SetInt("notarget", 1);

		idEntity* newEnt = NULL;
		gameLocal.SpawnEntityDef(dict, &newEnt);

		if (newEnt) 
		{
			gameLocal.Printf("Spawned Enemy Entity '%s' (%f, %f, %f)\n", newEnt->name.c_str(), origin.x, origin.y, origin.z);
			enemyUnits.Append(newEnt->entityNumber);
			savedSpawnPositions.Append(idVec3(origin.x, origin.y, origin.z));
			validPositionSelected = false;
		}
	}
}

void RoundManager::RemoveEnemyUnit(int entityNum) 
{
	enemyUnits.Remove(entityNum);
}

void RoundManager::AddPlayerUnit(int entityNum) 
{
	int index = playerUnits.Append(entityNum);
	gameLocal.Printf("Appended entity num %d, at index %d\n", entityNum, index);
}

void RoundManager::RemovePlayerUnit(int entityNum) 
{
	playerUnits.Remove(entityNum);
}

void RoundManager::ClearPlayerUnits()
{
	int playerUnitCount = playerUnits.Num();

	for (int i = 0; i < playerUnitCount; i++)
	{
		gameLocal.Printf("Fetching entity num %d at index %d\n", playerUnits[i], i);
		idEntity* playerUnit = gameLocal.entities[playerUnits[i]];

		if (playerUnit)
		{
			gameLocal.Printf("Deleting player unit %s\n", playerUnit->name.c_str());
			delete playerUnit;
		}
	}

	playerUnits.Clear();
}

void RoundManager::Think() 
{
	idPlayer* player = gameLocal.GetLocalPlayer();

	if (!player)
	{
		return;
	}

	if (roundEnded && !player->pfl.dead) 
	{
		if (roundStarted)
		{
			roundStarted = false;

			if (playerUnits.Num() == 0)
			{
				round = 1;
				player->Kill(false, false);

				return;
			}
			else if (enemyUnits.Num() == 0) 
			{
				round++;
				ClearPlayerUnits();
				player->Teleport(idVec3(7646, 12800, 2059), player->spawnAngles, NULL);
			}
		}

		player->hud->SetStateString("rtext", va("Round %d", round));
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