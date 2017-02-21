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

void PlayerClass::healthUI()
{
	/*----------Health System (Hearts)------*/
	//if (Application::IsKeyPressed('V'))
	//{
	//	if (_health > 0)
	//		_health -= 10;
	//}
	//if (Application::IsKeyPressed('C'))
	//{
	//	if (_health < 100)
	//		_health += 10;
	//}

	if (!isDead())
	{
		Hearts.heartCounter = _health / 10;

		if (Hearts.heartCounter == 10)
		{
			for (int i = 0; i < 10; i++)
			{
				Hearts.a_heart[i] = 2;
				Hearts.a_blankheart[i] = 0;
			}
		}
		else
		{
			for (int i = 0; i < Hearts.heartCounter; i++)
			{
				Hearts.a_heart[i] = 2;
				Hearts.a_blankheart[i] = 0;
			}
			for (int i = Hearts.heartCounter; i < 10; i++)
			{
				Hearts.a_blankheart[i] = 2;
				Hearts.a_heart[i] = 0;
			}
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
	if (Application::IsKeyPressed('A') && Application::IsKeyPressed('D'))
	{
	}
	else
	{
		if ((Application::IsKeyPressed('A') && a_LookingDirection == 90) ||
			(Application::IsKeyPressed('D') && a_LookingDirection == -90))
		{
			a_LookingDirection *= -1;
		}
	}
}

void PlayerClass::healthSystem(bool Block)
{
	if (Block)
		this->_health -= 10;
	else
		this->_health -= 20;
}