#ifndef BOSS_H
#define BOSS_H

#include "EnemyClass.h"
#include "PlayerClass.h"

class Boss : public EnemyClass
{
public:
	static Boss* get_instance()
	{
		if (!instance)
			instance = new Boss();
		else
			return instance;
	}

	struct BossLife
	{
		float boss_heart[30];
		int heartCounter;
	};

	void bossInit();
	void bossHealthUI();
	void bossHealthSystem();
	void facingDirection();
	void stateManager();
	void dmgOvertime(double timeElapsed);
	void burrowTeleportation(double timeElapsed);
	void spinAttack(double timeElapsed);
	void tailAttack(double timeElapsed);
	unsigned int get_health();
	BossLife bossLife;

private:
	Boss(): boss_health(300), boss_lookingDirection(90) {};
	~Boss() { 
		delete instance; };
	static Boss* instance;
	unsigned int boss_health;
	int boss_lookingDirection;
	double bufferTime_DoT;
	bool tailAtk;
	bool spin;
	bool burrow;
	bool DmgOverTime;
	bool magicImmunity;
};

#endif
