#ifndef PLAYERCLASS_H
#define PLAYERCLASS_H

#include "BoundingBox.h"

class PlayerClass
{
public:
	static PlayerClass* get_instance()
	{
		if (!instance)
			instance = new PlayerClass();
		else
			return instance;
	}

	unsigned int get_health();
	unsigned int get_energy();

	void healthUI();
	void manaUI();
	void manaSystem();
	void init();
	void facingDirection();
	void healthSystem(bool Block);
	void bossFightFacingDirection();

	struct PlayerLife
	{
		float a_heart[10];
		float a_blankheart[10];
		int heartCounter;

		float a_energy[10];
		float a_blankenergy[10];
		int energyCounter;
	};

	BoundingBox PlayerHitBox;
	Vector3 position_a;
	PlayerLife Hearts;
	int a_LookingDirection;
	int hm_Saved;
	int fm_Killed;

private:
	PlayerClass(): _health(100), _energy(100), a_LookingDirection(90), hm_Saved(0), fm_Killed(0) {};
	static PlayerClass* instance;

	unsigned int _health;
	unsigned int _energy;
	bool isDead();
};

#endif //PLAYERCLASS_H