#include "EnemyClassManager.h"

EnemyManager *EnemyManager::instance = 0;


void EnemyManager::spawnEnemy(Vector3 position)
{
	EnemyClass* Mutant = new EnemyClass(position);
	Mutant->init();
	instance->EnemyList.push_back(Mutant);
}