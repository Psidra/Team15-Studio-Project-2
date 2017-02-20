#ifndef ENEMYCLASSMANAGER_H
#define ENEMYCLASSMANAGER_H

#include <vector>
#include "EnemyClass.h"

class EnemyManager
{
public:
	static EnemyManager* get_instance()
	{
		if (!instance)
			instance = new EnemyManager();
		else
			return instance;
	}

	void spawnEnemy(Vector3 position);
	std::vector<EnemyClass*> EnemyList;

private:
	EnemyManager() {};
	~EnemyManager() {
		delete instance;
	};
	static EnemyManager* instance;
};

#endif // ENEMYCLASSMANAGER_H