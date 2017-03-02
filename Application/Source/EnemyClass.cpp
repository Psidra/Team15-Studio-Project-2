#include "EnemyClass.h"
#include "PlayerClass.h"
#include "Boss.h"
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
	if (this->health > 0)
	{
		this->detection();
		this->movement(dt);

		for (unsigned int projectiles = 0; projectiles < this->spit_.size(); projectiles++)
		{
			this->spit_[projectiles]->init_proj(this->position_m.x - PlayerClass::get_instance()->position_a.x);
		}

		this->proj_update(false);
	}
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
			position_m.x += (float)(10.f * dt * movementDirection);
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

void EnemyClass::attack(unsigned int projType, Vector3 pos, Vector3 dir, double dt, bool block)
{
	if (this->projectileThrowRange)
		this->spit_.push_back(projectileBuilder::GenerateProjectile(projType, pos, dir));
	else if (this->meleeAtkRange)
	{
		if (this->EnemyHitBox.collide(PlayerClass::get_instance()->PlayerHitBox)) // can't roll dodge this I think
			PlayerClass::get_instance()->healthSystem(block, false);
	}
	else
	{
		// jackshit
	}
}

void EnemyClass::detection()
{
	float distBetweenThem = this->position_m.x - PlayerClass::get_instance()->position_a.x;

	if ((distBetweenThem <= 60.f && distBetweenThem >= 30.f) || // Detect the player but stand at its spot  (60 to 30) 
		(distBetweenThem <= -30.f && distBetweenThem >= -60.f)) // and changes it to throwing projectile state (-30 to -60)
	{
		projectileThrowRange = true;
	}
	else
	{
		projectileThrowRange = false;
	}

	if ((distBetweenThem <= 30.f && distBetweenThem >= 5.5f) || // Change state to move towards player (29 to 10) (-10 to -29)
		(distBetweenThem <= -5.5f && distBetweenThem >= -30.f))
	{
		moveRange = true;
	}
	else
	{
		moveRange = false;
	}

	if (distBetweenThem <= 5.5f && distBetweenThem >= -5.5f) // Change state to melee attack (9 to -9)
	{
		meleeAtkRange = true;
	}
	else
	{
		meleeAtkRange = false;
	}
}

void EnemyClass::proj_update(bool boss)
{
	for (unsigned int projectiles = 0; projectiles < this->spit_.size(); projectiles++)
	{
		unsigned int fireworks = rand() % 10 + 1;

		if (boss)
		{
			if (this->spit_[projectiles]->projType_ == 1)
			{
				if (this->spit_[projectiles]->displacement() < 7.f)
					this->spit_[projectiles]->position_.y -= 2.f;

				this->spit_[projectiles]->boss_edit_passed_angle(); // boss_edit_passed_angle();
				this->spit_[projectiles]->direction_.y = sin(this->spit_[projectiles]->get_passed_angle());/*Math::RadianToDegree(sin(this->spit_[projectiles]->get_passed_angle()));*/
			}
			if (this->spit_[projectiles]->projType_ == 2)
			{
				if (this->spit_[projectiles]->displacement() < 7.f)
					this->spit_[projectiles]->position_.y -= 1.f;

				this->spit_[projectiles]->boss_edit_passed_angle();
				this->spit_[projectiles]->direction_.y = -sin(this->spit_[projectiles]->get_passed_angle());
			}
			if (this->spit_[projectiles]->projType_ == 3)
			{
				if (this->spit_[projectiles]->displacement() < 13.f)
					this->spit_[projectiles]->position_.y -= 1.f;

				this->spit_[projectiles]->direction_.y = 0;
			}
			if (this->spit_[projectiles]->projType_ == 4)
			{
				this->spit_[projectiles]->edit_passed_angle();
				this->spit_[projectiles]->direction_.y = -tan(this->spit_[projectiles]->get_passed_angle());
				this->spit_[projectiles]->projSpeed = 0.5f;
			}

			this->spit_[projectiles]->position_ += (this->spit_[projectiles]->direction_ * this->spit_[projectiles]->projSpeed);
		}
		else
		{
			if (this->spit_[projectiles]->projType_ == 1)
			{
				this->spit_[projectiles]->edit_passed_angle();
				this->spit_[projectiles]->direction_.y = sin(this->spit_[projectiles]->get_passed_angle());/*Math::RadianToDegree(sin(this->spit_[projectiles]->get_passed_angle()));*/
			}

			this->spit_[projectiles]->position_ += (this->spit_[projectiles]->direction_ * this->spit_[projectiles]->projSpeed);
		}
	}
}

void EnemyClass::edit_health(int HP)
{
	this->health += HP;
}


void EnemyClass::restartLevel()
{
	health = 100;
}

int EnemyClass::get_health()
{
	return health;
}

unsigned int EnemyClass::get_action()
{
	if (meleeAtkRange)
		return 1;
	else if (projectileThrowRange)
		return 2;
	else
		return 0;
}