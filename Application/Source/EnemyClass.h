#ifndef ENEMYCLASS_H
#define ENEMYCLASS_H

#include <vector>
#include "Projectile.h"
#include "BoundingBox.h"

class EnemyClass
{
public:
	EnemyClass() {};
	EnemyClass(Vector3 pos) : position_m(pos) {};
	~EnemyClass() {};

	void init();
	void update(double dt);
	void movement(double dt);
	void attack(unsigned int projType, Vector3 pos, Vector3 dir, double dt, bool block);
	void detection();
	void edit_health(int HP);
	void restartLevel();
	int get_health();
	void proj_update(bool boss);
	unsigned int get_action();
	double M_et[5]; // heh "met"
	double bufferTime_attack_MC;

	std::vector<Projectile*> spit_;

	BoundingBox EnemyHitBox;
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