#include "PlayerClass.h"

void PlayerClass::add_health(int HP)
{
	if (_health < 5)
		_health += HP;
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