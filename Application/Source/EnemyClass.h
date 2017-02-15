#ifndef ENEMYCLASS_H
#define ENEMYCLASS_H

#include "BoundingBox.h"

class EnemyClass
{
public:
	EnemyClass();
	~EnemyClass();

	unsigned int get_health();


private:
	unsigned int _health;

};

#endif //EnemyClass_H