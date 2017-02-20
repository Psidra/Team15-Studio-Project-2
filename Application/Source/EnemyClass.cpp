#include "EnemyClass.h"
#include "PlayerClass.h"
#include "StudioProject2_Scene1.h"
#include "Projectile.h"
#include "ProjectileBuilder.h"

EnemyClass::EnemyClass() : _health(100), meleeAtkRange(false), moveRange(false), projectileThrowRange(false)
{
	
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
		if ((position_m.x - PlayerClass::get_instance()->position_a.x) > 5)
		{
			position_m.x -= (float)(20.f * dt);
			lookRight = false;
		}
		else if ((position_m.x - PlayerClass::get_instance()->position_a.x) < -5)
		{
			position_m.x += (float)(20.f * dt);
			lookRight = true;
		}
	}
	else // idle movement
	{
		if (meleeAtkRange || projectileThrowRange)
		{
			if (position_m.x < PlayerClass::get_instance()->position_a.x)
				lookRight = true;
			else
				lookRight = false;
		}
		else
		{
			static float movementDirection = 1.f;
			position_m.x += (float)(20.f * dt * movementDirection);
			if ((position_m.x > positionStorageX1 && movementDirection != -1.f) || (position_m.x < positionStorageX2 && movementDirection != 1.f))
			{
				movementDirection *= -1;
			}

			if (movementDirection == 1)
				lookRight = true;
			else
				lookRight = false;
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
		float distBetweenThem = position_m.x - PlayerClass::get_instance()->position_a.x;

		if ((distBetweenThem < 61.f && distBetweenThem > 29.f) || // Detect the player but stand at its spot  (60 to 30) 
			(distBetweenThem < -29.f && distBetweenThem > -61.f)) // and changes it to throwing projectile state (-30 to -60)
		{
			projectileThrowRange = true;
		}
		else
		{
			projectileThrowRange = false;
		}

		if ((distBetweenThem < 30.f && distBetweenThem > 5.f) || // Change state to move towards player (29 to 10) (-10 to -29)
			(distBetweenThem < -5.f && distBetweenThem > -30.f))
		{
			moveRange = true;
		}
		else
		{
			moveRange = false;
		}

		if (distBetweenThem < 5.f && distBetweenThem > -5.f) // Change state to melee attack (9 to -9)
		{
			meleeAtkRange = true;
		}
		else
		{
			meleeAtkRange = false;
		}
	}
}

void EnemyClass::init()
{
	positionStorageX1 = position_m.x + 30;
	positionStorageX2 = position_m.x - 30;
	enemyLookingDir = 0;
}

void EnemyClass::rangedattack(unsigned int projType, Vector3 pos, double dt)
{
	Projectile* GenerateProjectile;
	spit_.push_back(GenerateProjectile);
}

void EnemyClass::facingDirection()
{
	if (enemyLookingDir == 0 && !lookRight)
	{
		enemyLookingDir = 180;
	}
	if (enemyLookingDir == 180 && lookRight)
	{
		enemyLookingDir = 0;
	}
}