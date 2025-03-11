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
	float yaw = 0;

	bool validPositionSelected = false;

	for (int i = 0; i < round; i++) 
	{
		int monster_idx = gameLocal.random.RandomInt(round / 4);

		dict.Set("classname", va("%s", units[monster_idx]));
		dict.Set("angle", va("%f", yaw));

		while (!validPositionSelected) 
		{
			float randomOffsetX = gameLocal.random.CRandomFloat();
			float randomOffsetY = gameLocal.random.CRandomFloat();

			float xRight = randomOffsetX > 0 ? 1 : -1;
			float yRight = randomOffsetY > 0 ? 1 : -1;

			idVec3 offset(xRight * (150 + (10 * round * randomOffsetX)), yRight * (150 + (10 * round * randomOffsetY)), 0);
			origin = CENTER_SPAWN_POINT + offset;

			gameLocal.Printf("Offset (%f, %f, %f)\n", offset.x, offset.y, offset.z);
			savedSpawnPositions.GetBool(va("%f%f%f", origin.x, origin.y, origin.z), "1", validPositionSelected);
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
			savedSpawnPositions.SetBool(va("%f%f%f", origin.x, origin.y, origin.z), false);
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
	playerUnits.Append(entityNum);
}

void RoundManager::RemovePlayerUnit(int entityNum) 
{
	playerUnits.Remove(entityNum);
}

void RoundManager::ClearPlayerUnits() 
{
	for (int i = 0; i < playerUnits.Num(); i++)
	{
		idEntity* playerUnit = gameLocal.entities[playerUnits[i]];
		RemovePlayerUnit(playerUnits[i]);

		if (playerUnit)
		{
			playerUnit->PostEventMS(&EV_Remove, 0);
		}
	}
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