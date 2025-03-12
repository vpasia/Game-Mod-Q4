#ifndef __ROUNDMANAGER_H_
#define __ROUNDMANAGER_H_

const idVec3 CENTER_SPAWN_POINT(6833, 13451, 381);

class RoundManager 
{
	public:
		int round;
		bool roundStarted;
		bool roundEnded;

		idList<int> playerUnits;
		idList<int> enemyUnits;

		void Init();

		void SpawnEnemyUnits();
		void RemoveEnemyUnit(int entityNum);

		void AddPlayerUnit(int entityNum);
		void RemovePlayerUnit(int entityNum);
		void ClearPlayerUnits();
		
		void StartRound();
		bool CheckEndRound();

		void Think();
};

#endif