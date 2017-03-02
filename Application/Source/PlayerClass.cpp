#include "PlayerClass.h"
#include "Application.h"
#include "Boss.h"
#include "GLFW\glfw3.h"
PlayerClass* PlayerClass::instance;

int PlayerClass::get_health()
{
	return _health;
}

int PlayerClass::get_energy()
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

void PlayerClass::healthSystem(bool Block, bool bossTagged)
{
	if (!bossTagged)
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
			this->_health -= 40;
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


void PlayerClass::energySystem()
{
	if (this->_energy < 100)
		this->_energy += 10;
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

void PlayerClass::laserBeam(double timeElapsed)
{
	if (glfwJoystickPresent(GLFW_JOYSTICK_1))
	{
		int xbox;
		const unsigned char *xbox360 = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &xbox);
		if ((GLFW_PRESS == xbox360[4] || Application::IsKeyPressed('Q')) && timeElapsed > bufferTime_Laser && _energy >= energyLaser) // Laser Beam
		{
			laserActive = true;
			bufferTime_Laser = timeElapsed + 10.f;
			_energy -= energyLaser;

		}
	}
	else
	{
		if (Application::IsKeyPressed('Q') && timeElapsed > bufferTime_Laser && _energy >= energyLaser) // Laser Beam
		{
			laserActive = true;
			bufferTime_Laser = timeElapsed + 10.f;
			_energy -= energyLaser;
		}
	}
		if (laserActive == true)
		{
			laserSize.x = 50;
			laserSize.y = 1;
			laserSize.z = 1;
			laserTranslate.z = 52;
			laserTranslate.y = 5;
			laserTranslate.x = 0;

			if (timeElapsed > bufferTime_Laser - 9)
			{
				laserActive = false;
			}
		}
		else
		{
			laserSize.x = 0.1;
			laserSize.y = 0.1;
			laserSize.z = 0.1;
			laserTranslate.z = 0;
			laserTranslate.y = 5;
			laserTranslate.x = 0;
		}
}

void PlayerClass::spellUI(double timeElapsed)
{
	if (_energy < energyLaser || timeElapsed < bufferTime_Laser || Boss::get_instance()->magicImmunity == true) // not enough energy or in cooldown or when boss magic immune in effect
	{
		spellHUD.laserNotReady = 5.f;
		spellHUD.laserReady = 0.f;
	}
	else
	{
		spellHUD.laserNotReady = 0.f;
		spellHUD.laserReady = 5.f;
	}

	if (_energy < energyProjShield || timeElapsed < bufferTime_ProjShield || Boss::get_instance()->magicImmunity == true)
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

void PlayerClass::healthRegeneration(double timeElapsed)
{
	if (_health < 100 && bufferTime_healthRegen < timeElapsed)
	{
		_health += 10;
		bufferTime_healthRegen = timeElapsed + 20.f; // Regenerate 1 heart every 20 seconds
	}
}

void PlayerClass::projectileShield(double timeElapsed, double dt)
{
	if (glfwJoystickPresent(GLFW_JOYSTICK_1))
	{
		int xbox;
		const unsigned char *xboxs = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &xbox);
		if ((Application::IsKeyPressed('R') || GLFW_PRESS == xboxs[5])
			&& bufferTime_ProjShield < timeElapsed && _energy >= energyProjShield)
		{
			projSheildActive = true;
			_energy -= energyProjShield;
			bufferTime_ProjShield = timeElapsed + 9.f;
		}
	}
	else
	{
		if (Application::IsKeyPressed('R') && bufferTime_ProjShield < timeElapsed && _energy >= energyProjShield)
		{
			projSheildActive = true;
			_energy -= energyProjShield;
			bufferTime_ProjShield = timeElapsed + 9.f;
		}
	}
	
	if (projSheildActive == true)
	{
		if (ProjShieldSize.x < 20 && ProjShieldSize.y < 20 && ProjShieldSize.z < 20)
		{
			ProjShieldSize.x += (5.0f * dt);
			ProjShieldSize.y += (5.0f * dt);
			ProjShieldSize.z += (5.0f * dt);
		}
		else
		{
			projSheildActive = false;
		}
	}
	else
	{
		ProjShieldSize.x = 0.1;
		ProjShieldSize.y = 0.1;
		ProjShieldSize.z = 0.1;
	}
}

unsigned int PlayerClass::get_killedstorage()
{
	return this->fmKilledStorage;
}

double PlayerClass::get_shielddur()
{
	return this->bufferTime_ProjShield;
}