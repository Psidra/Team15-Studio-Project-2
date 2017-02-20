#ifndef ENEMYCLASS_H
#define ENEMYCLASS_H

#include "BoundingBox.h"
#include "Projectile.h"
#include "Vector3.h"
#include <vector>

class EnemyClass
{
public:
	EnemyClass();
	~EnemyClass();
	void battle();
	void detection();
	void movement(double dt);
	void update();

	std::vector<Projectile*> spit_;
	void rangedattack(unsigned int projType, Vector3 pos, double dt); // mutants be spitting straight fire yo

	unsigned int get_currHealth();

	Vector3 position_m;

private:
	float positionStorageX1;
	float positionStorageX2;
	unsigned int _health;
	bool isDead();
	bool moveRange;
	bool meleeAtkRange;
	bool projectileThrowRange;
};

#endif //EnemyClass_H