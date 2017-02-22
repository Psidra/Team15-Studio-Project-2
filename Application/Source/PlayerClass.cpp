#include "PlayerClass.h"
#include "Application.h"

PlayerClass* PlayerClass::instance;

unsigned int PlayerClass::get_health()
{
	return _health;
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

void PlayerClass::bossFightFacingDirection()
{
	if ((Application::IsKeyPressed('A') && Application::IsKeyPressed('D')) ||
		(Application::IsKeyPressed('W') && Application::IsKeyPressed('S')))
	{
	}
	else
	{	// W-A , W-D, S-A, S-D, W, A, S, D
		if (Application::IsKeyPressed('A') && Application::IsKeyPressed('W'))
			a_LookingDirection = -45.f;
		if (Application::IsKeyPressed('A') && Application::IsKeyPressed('S'))
			a_LookingDirection = 45.f;
		if (Application::IsKeyPressed('D') && Application::IsKeyPressed('W'))
			a_LookingDirection = -135.f;
		if (Application::IsKeyPressed('D') && Application::IsKeyPressed('S'))
			a_LookingDirection = 135.f;
		if (Application::IsKeyPressed('W'))
			a_LookingDirection = -90.f;
		if (Application::IsKeyPressed('S'))
			a_LookingDirection = 90.f;
		if (Application::IsKeyPressed('A'))
			a_LookingDirection = 0.f;
		if (Application::IsKeyPressed('D'))
			a_LookingDirection = 180.f;
	}
}

void PlayerClass::manaUI()
{
	if (!isDead())
	{
		Hearts.energyCounter = _energy / 10;

		if (Hearts.energyCounter == 10)
		{
			for (int i = 0; i < 10; i++)
			{
				Hearts.a_energy[i] = 2;
				Hearts.a_blankenergy[i] = 0;
			}
		}
		else
		{
			for (int i = 0; i < Hearts.energyCounter; i++)
			{
				Hearts.a_energy[i] = 2;
				Hearts.a_blankenergy[i] = 0;
			}
			for (int i = Hearts.energyCounter; i < 10; i++)
			{
				Hearts.a_blankenergy[i] = 2;
				Hearts.a_energy[i] = 0;
			}
		}
	}
}

void PlayerClass::manaSystem()
{

}