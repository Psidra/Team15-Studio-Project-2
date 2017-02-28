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

		return instance;
	}

	struct BossLife
	{
		float boss_heart[30];
		int heartCounter;
	};

	void bossInit();
	void bossHealthUI();
	void bossHealthSystem(double timeElapsed);
	void facingDirection();
	void stateManager();
	void dmgOvertime(double timeElapsed);
	void burrowTeleportation(double timeElapsed);
	void spinAttack(double timeElapsed, bool block);
	void tailAttack(double timeElapsed, bool block);
	void update(double timeElapsed, bool block);
	unsigned int get_health();
	unsigned int get_action();
	void proj_attack(Vector3 pos, Vector3 dir, double elapsedTime);
	unsigned int get_pattern();
	void boss_attack(double elapsedTime, bool block);

	BossLife bossLife;
	Tail Boss_Tail; // great now tail doesn't even look like a word anymore, thanks semanic satiation
	bool magicImmunity;

private:
	Boss(): boss_health(300), boss_lookingDirection(90) {};
	~Boss() { 
		delete instance; };
	static Boss* instance;
	unsigned int boss_health;
	int boss_lookingDirection;
	unsigned int attack_pattern;
	double bufferTime_DoT;
	double bufferTime_atk;
	double cooldown_Burrow;
	double cooldown_Spin;
	double spinningDuration;
	double bufferTime_tail;
	double bufferTime_attackpattern;
	double bufferTime_iframe;
	double bufferTime_attackchoice;
	double bufferTime_projduration;
	unsigned int attackchoice;
	float direction_p;
	float offset = 0.f;
	/*-----For State Control---*/
	bool tailAtk; 
	bool spin;
	bool burrow;
	bool DmgOverTime;
	/*------------------------*/
	int random_choice;
	
	/*----For preventing continuous update--*/
	bool burrowDir;
	bool burrowDist;
	/*--------------------------------------*/

	/*-----For preventing multiple actions---*/
	bool spinning;
	bool burrowing;
	bool tailattacking;
	bool projattacking;
};

#endif
