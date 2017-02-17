#ifndef ENEMYCLASS_H
#define ENEMYCLASS_H

#include "BoundingBox.h"

class EnemyClass
{
public:
	EnemyClass();
	~EnemyClass();
	void battle();
	void detection();
	void movement();

	unsigned int get_currHealth();

	struct Coordinates
	{
		float posX;
		float posY;
		float posZ;
	};

	Coordinates EnemyPos;

private:
	unsigned int _health;
	bool isDead();
	bool detectRange;
	bool atkRange;

};

#endif //EnemyClass_H