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

	void add_energy(int EP);
	unsigned int get_energy();

	void movement();
	void healthUI();
	void init();
	void facingDirection();
	void healthSystem();

	struct PlayerLife
	{
		float a_heart[10];
		float a_blankheart[10];
		int heartCounter;
	};

	BoundingBox PlayerHitBox;
	Vector3 position_a;
	PlayerLife Hearts;
	int a_LookingDirection;

private:
	PlayerClass(): _health(100), a_LookingDirection(90) {};
	static PlayerClass* instance;

	unsigned int _health;
	unsigned int _energy;
	bool isDead();
};

#endif //PLAYERCLASS_H