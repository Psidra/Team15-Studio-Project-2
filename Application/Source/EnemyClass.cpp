#include "EnemyClass.h"
#include "PlayerClass.h"
#include "Projectile.h"
#include "ProjectileBuilder.h"

void EnemyClass::init()
{
	health = 100;
	direction_m.x = -1;
	positionStorageX1 = position_m.x + 30;
	positionStorageX2 = position_m.x - 30;
}

void EnemyClass::update(double dt)
{
	this->detection();
	this->movement(dt);
	this->proj_update();
}

void EnemyClass::movement(double dt)
{
	if (moveRange == true)
	{
		if ((position_m.x - PlayerClass::get_instance()->position_a.x) > 5)
		{
			position_m.x -= (float)(20.f * dt);
			direction_m.x = -1;
		}
		else if ((position_m.x - PlayerClass::get_instance()->position_a.x) < -5)
		{
			position_m.x += (float)(20.f * dt);
			direction_m.x = 1;
		}
	}
	else // idle movement
	{
		if (meleeAtkRange || projectileThrowRange)
		{
			if (position_m.x < PlayerClass::get_instance()->position_a.x)
				direction_m.x = 1;
			else
				direction_m.x = -1;
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
				direction_m.x = 1;
			else
				direction_m.x = -1;
		}
	}
}

void EnemyClass::attack(bool isranged, unsigned int projType, Vector3 pos, Vector3 dir, double dt)
{
	if (isranged)
	{
		this->spit_.push_back(projectileBuilder::GenerateProjectile(projType, pos, dir));
	}
	else
	{
		// do some shit
	}
}
void EnemyClass::detection()
{
	if (health > 0)
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

void EnemyClass::proj_update()
{
	for (unsigned int projectiles = 0; projectiles < this->spit_.size(); projectiles++)
	{
		this->spit_[projectiles]->position_ += (this->spit_[projectiles]->direction_ * this->spit_[projectiles]->projSpeed);
	}
}