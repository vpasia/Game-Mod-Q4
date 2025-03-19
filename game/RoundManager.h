#ifndef __ROUNDMANAGER_H_
#define __ROUNDMANAGER_H_

const idVec3 CENTER_SPAWN_POINT(6833, 13451, 381);

class RoundManager 
{
	public:
		int round;
		int roundEcon;
		bool roundStarted;
		bool roundEnded;
		bool playerLost;

		idList<int> playerUnits;
		idList<int> enemyUnits;

		void Init();

		void SpawnEnemyUnits();
		void RemoveEnemyUnit(int entityNum);
		void ClearEnemyUnits();

		void AddPlayerUnit(int entityNum);
		void RemovePlayerUnit(int entityNum);
		void ClearPlayerUnits();
		
		void StartRound();
		bool CheckEndRound();

		bool SpendEcon(int index);
		void SetRoundEcon();

		void Think();
};

#endif