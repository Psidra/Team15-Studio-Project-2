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
	void movement(double dt);
	void update();
	void facingDirection();

	unsigned int get_currHealth();

	struct Coordinates
	{
		float posX;
		float posY;
		float posZ;
	};

	Coordinates EnemyPos;
	int enemyLookingDir;

private:
	float positionStorageX1;
	float positionStorageX2;
	unsigned int _health;
	bool isDead();
	bool moveRange;
	bool meleeAtkRange;
	bool projectileThrowRange;
	bool lookRight;
};

#endif //EnemyClass_H