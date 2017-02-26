#ifndef BOSS_H
#define BOSS_H

#include "EnemyClass.h"
#include "PlayerClass.h"
#include "Tail.h"

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
	void spinAttack(double timeElapsed, bool block);
	void tailAttack(double timeElapsed, bool block);
	unsigned int get_health();
	BossLife bossLife;
	Tail* Boss_Tail; // great now tail doesn't even look like a word anymore, thanks semanic satiation

private:
	Boss(): boss_health(300), boss_lookingDirection(90) {};
	~Boss() { 
		delete instance; };
	static Boss* instance;
	unsigned int boss_health;
	int boss_lookingDirection;
	double bufferTime_DoT;
	double cooldown_Burrow;
	double cooldown_Spin;
	double spinningDuration;
	double bufferTime_tail;
	/*-----For State Control---*/
	bool tailAtk; 
	bool spin;
	bool burrow;
	bool DmgOverTime;
	bool magicImmunity;
	/*------------------------*/
	
	/*----For preventing continuous update--*/
	bool burrowDir;
	bool burrowDist;
	/*--------------------------------------*/

	/*-----For preventing multiple actions---*/
	bool spinning;
	bool burrowing;
	bool tailattacking;
};

#endif
