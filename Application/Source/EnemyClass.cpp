#include "EnemyClass.h"
#include "PlayerClass.h"
#include "StudioProject2_Scene1.h"

EnemyClass::EnemyClass()
{
	_health = 100;
	meleeAtkRange = false;
	moveRange = false;
	projectileThrowRange = false;
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
	if (meleeAtkRange == true)
	{

	}
	else
	{
		
	}
}

void EnemyClass::movement(double dt)
{
	if (moveRange == true)
	{
		if ((EnemyPos.posX - PlayerClass::get_instance()->Coord.posX) > 5)
		{
			EnemyPos.posX -= (float)(30.f * dt);
		}
		else if ((EnemyPos.posX - PlayerClass::get_instance()->Coord.posX) < -5)
		{
			EnemyPos.posX += (float)(30.f * dt);
		}
	}
	else // idle movement
	{
		if (projectileThrowRange == true) // when throwing projectile, shouldnt move
		{
		}
		else // when not throwing projectile then move
		{
			static float movementDirection = 1.f;
			EnemyPos.posX += (float)(20.f * dt * movementDirection);
			if ((EnemyPos.posX > positionStorageX1 && movementDirection != -1.f)|| (EnemyPos.posX < positionStorageX2 && movementDirection != 1.f))
			{
					movementDirection *= -1;
			}
		}
	}
}

unsigned int EnemyClass::get_currHealth()
{
	return _health;
}

void EnemyClass::detection()
{
	if (!isDead())
	{
		float distBetweenThem = EnemyPos.posX - PlayerClass::get_instance()->Coord.posX;

		if ((distBetweenThem < 61.f && distBetweenThem > 29.f) || // Detect the player but stand at its spot  (60 to 30) 
			(distBetweenThem < -29.f && distBetweenThem > -61.f)) // and changes it to throwing projectile state (-30 to -60)
		{
			projectileThrowRange = true;
		}
		else
		{
			projectileThrowRange = false;
		}

		if ((distBetweenThem < 30.f && distBetweenThem > 3.f) || // Change state to move towards player (29 to 10) (-10 to -29)
			(distBetweenThem < -3.f && distBetweenThem > -30.f))
		{
			moveRange = true;
		}
		else
		{
			moveRange = false;
		}

		if (distBetweenThem < 3.f && distBetweenThem > -3.f) // Change state to melee attack (9 to -9)
		{
			meleeAtkRange = true;
		}
		else
		{
			meleeAtkRange = false;
		}
	}
}

void EnemyClass::update()
{
	positionStorageX1 = EnemyPos.posX + 30;
	positionStorageX2 = EnemyPos.posX - 30;
}