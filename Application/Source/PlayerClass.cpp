#include "PlayerClass.h"
#include "Application.h"
#include "GLFW\glfw3.h"
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

		if (Application::IsKeyPressed('P'))
			_health = 0;
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
	int Connected = glfwJoystickPresent(GLFW_JOYSTICK_1);

	if (Connected == 1)
	{
		int buttonCount;
		const unsigned char *xbox = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
		if (Application::IsKeyPressed('A') && Application::IsKeyPressed('D'))
		{
		}
		else
		{
			if (((Application::IsKeyPressed('A') || GLFW_PRESS == xbox[13]) && a_LookingDirection == 90) ||
				((Application::IsKeyPressed('D') || GLFW_PRESS == xbox[11]) && a_LookingDirection == -90))
			{
				a_LookingDirection *= -1;
			}
		}
	}
	else
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
}

void PlayerClass::healthSystem(bool Block)
{
	if (bossFight == false) // for regular scene
	{
		if (Block)
			this->_health -= 10;
		else
			this->_health -= 20;
	}
	else
	{
		if (Block)
			this->_health -= 20;
		else
			this->_health -= 30;
	}
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

void PlayerClass::timeSpent(float dt)
{
	timeSpend += dt;
}

void PlayerClass::restartLevel()
{
	_health = healthStorage;
	_energy = energyStorage;
	hm_Saved = hmSavedStorage;
	fm_Killed = fmKilledStorage;
}

void PlayerClass::init()
{
	healthStorage = _health;
	energyStorage = _energy;
	hmSavedStorage = hm_Saved;
	fmKilledStorage = fm_Killed;
}

void PlayerClass::restartGame()
{
	_health = 100;
	_energy = 100;
	hm_Saved = 0;
	fm_Killed = 0;
}

void PlayerClass::spells(double timeElapsed)
{
	if (_energy > 0)
	{
		if (Application::IsKeyPressed('Q') && timeElapsed > bufferTime_Laser) // Laser Beam
		{
			_energy -= energyLaser;
			bufferTime_Laser = timeElapsed + 10.f;
		}

		if (Application::IsKeyPressed('R') && timeElapsed > bufferTime_ProjShield) // Projectile Shield
		{
			_energy -= energyProjShield;
			bufferTime_ProjShield = timeElapsed + 7.f; 
		}
	}
}

void PlayerClass::spellUI(double timeElapsed)
{
	if (_energy < energyLaser || timeElapsed < bufferTime_Laser) // not enough energy or in cooldown
	{
		spellHUD.laserNotReady = 5.f;
		spellHUD.laserReady = 0.f;
	}
	else
	{
		spellHUD.laserNotReady = 0.f;
		spellHUD.laserReady = 5.f;
	}

	if (_energy < energyProjShield || timeElapsed < bufferTime_ProjShield)
	{
		spellHUD.projShieldNotReady = 5.f;
		spellHUD.projShieldReady = 0.f;
	}
	else
	{
		spellHUD.projShieldNotReady = 0.f;
		spellHUD.projShieldReady = 5.f;
	}
}