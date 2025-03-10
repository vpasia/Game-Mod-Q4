#ifndef __ROUNDMANAGER_H_
#define __ROUNDMANAGER_H_

class RoundManager 
{
	private:
		int round = 1;

		bool roundStarted;

		idList<idEntity*> playerUnits;
		idList<idEntity*> enemyUnits;

	public:
		int GetRound();
		void SpawnEnemyUnits();
		void AddPlayerUnit(idEntity* unit);
		bool CheckEndRound();
};

#endif