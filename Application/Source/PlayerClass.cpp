#include "PlayerClass.h"
#include "Application.h"

PlayerClass* PlayerClass::instance;

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

void PlayerClass::healthSystem()
{
	/*----------Health System (Hearts)------*/
	if (Application::IsKeyPressed('V'))
	{
		_health -= 10;
	}
	if (Application::IsKeyPressed('C'))
	{
		_health += 10;
	}

	if (!isDead())
	{
		Hearts.heartCounter = _health / 10;

		switch (Hearts.heartCounter)
		{
		case 0:
			Hearts.a_blankheart[0] = 2;
			Hearts.a_heart[0] = 0;
			break;
		case 1:
			Hearts.a_blankheart[0] = 0;
			Hearts.a_heart[0] = 2;
			Hearts.a_blankheart[1] = 2;
			Hearts.a_heart[1] = 0;
			break;
		case 2:
			Hearts.a_blankheart[1] = 0;
			Hearts.a_heart[1] = 2;
			Hearts.a_blankheart[2] = 2;
			Hearts.a_heart[2] = 0;
			break;
		case 3:
			Hearts.a_blankheart[2] = 0;
			Hearts.a_heart[2] = 2;
			Hearts.a_blankheart[3] = 2;
			Hearts.a_heart[3] = 0;
			break;
		case 4:
			Hearts.a_blankheart[3] = 0;
			Hearts.a_heart[3] = 2;
			Hearts.a_blankheart[4] = 2;
			Hearts.a_heart[4] = 0;
			break;
		case 5:
			Hearts.a_blankheart[4] = 0;
			Hearts.a_heart[4] = 2;
			Hearts.a_blankheart[5] = 2;
			Hearts.a_heart[5] = 0;
			break;
		case 6:
			Hearts.a_blankheart[5] = 0;
			Hearts.a_heart[5] = 2;
			Hearts.a_blankheart[6] = 2;
			Hearts.a_heart[6] = 0;
			break;
		case 7:
			Hearts.a_blankheart[6] = 0;
			Hearts.a_heart[6] = 2;
			Hearts.a_blankheart[7] = 2;
			Hearts.a_heart[7] = 0;
			break;
		case 8:
			Hearts.a_blankheart[7] = 0;
			Hearts.a_heart[7] = 2;
			Hearts.a_blankheart[8] = 2;
			Hearts.a_heart[8] = 0;
			break;
		case 9:
			Hearts.a_blankheart[8] = 0;
			Hearts.a_heart[8] = 2;
			Hearts.a_blankheart[9] = 2;
			Hearts.a_heart[9] = 0;
			break;
		case 10:
			Hearts.a_blankheart[9] = 0;
			Hearts.a_heart[9] = 2;
			break;
		}
	}

	
	/*--------------------------------------*/
}

bool PlayerClass::isDead()
{
	if (_health < 0)
		return true;
	else
		return false;
}

void PlayerClass::facingDirection()
{
	if ((Application::IsKeyPressed('A') && a_LookingDirection == 90) ||
		(Application::IsKeyPressed('D') && a_LookingDirection == -90))
	{
		a_LookingDirection *= -1;
	}
}