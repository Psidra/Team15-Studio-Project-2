#ifndef ENEMYCLASS_H
#define ENEMYCLASS_H

#include <vector>
#include "Projectile.h"

class EnemyClass
{
public:
	EnemyClass() {};
	EnemyClass(Vector3 pos) : position_m(pos) {};
	~EnemyClass() {};

	void init();
	void update(double dt);
	void movement(double dt);
	void attack(bool ranged, unsigned int projType, Vector3 pos, Vector3 dir, double dt);
	void detection();

	std::vector<Projectile*> spit_;

	Vector3 position_m;
	Vector3 direction_m;

private:
	unsigned int health;
	float positionStorageX1;
	float positionStorageX2;

	bool moveRange;
	bool meleeAtkRange;
	bool projectileThrowRange;
};

#endif // ENEMYCLASS_H