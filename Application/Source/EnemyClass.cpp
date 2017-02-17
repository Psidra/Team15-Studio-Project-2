#include "EnemyClass.h"
#include "PlayerClass.h"


EnemyClass::EnemyClass()
{
	_health = 100;
	atkRange = false;
	detectRange = false;
}

EnemyClass::~EnemyClass()
{

}

bool EnemyClass::isDead()
{
	if (_health <= 0)
		return true;
	else
		return false;
}

void EnemyClass::battle()
{
	if (!isDead())
	{
		if (atkRange == true)
		{

		}
		else
		{

		}
	}
}

void EnemyClass::movement()
{
	if (!isDead())
	{
		
	}
}

unsigned int EnemyClass::get_currHealth()
{
	return _health;
}

void EnemyClass::detection()
{
	//int distBetweenThem =  

	/*if (!isDead())
	{

	}*/
}
