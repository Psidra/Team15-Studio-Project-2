#include "PlayerClass.h"

PlayerClass* PlayerClass::instance;

void PlayerClass::edit_health(int HP)
{
	if (HP > 0)
	{
		if (_health < 5)
			_health += HP;
	}
	else
	{
		if (_health < 0)
			_dead = true;
		// do some other shit here since he's dead idk have a screen of death
	}
}

unsigned int PlayerClass::get_health()
{
	return _health;
}

void PlayerClass::add_energy(int EP)
{
	if (_energy < 10)
		_energy += EP;
}

unsigned int PlayerClass::get_energy()
{
	return _energy;
}