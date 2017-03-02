#include "Boss.h"
#include "PlayerClass.h"
#include "Application.h"
#include "ProjectileBuilder.h"
#include <time.h>
#include <stdlib.h>

Boss* Boss::instance;

unsigned int Boss::get_health()
{
	return boss_health;
}

void Boss::bossInit()
{
	get_instance()->position_m = Vector3(350, 3, 0);
	this->EnemyHitBox.setto(get_instance()->position_m.x, get_instance()->position_m.y, get_instance()->position_m.z);
	spin = false;
	tailAtk = false;
	burrow = false;
	DmgOverTime = false;
	magicImmunity = false;
	burrowDist = false;
	burrowDir = false;
	tailattacking = false;
	burrowing = false;
	spinning = false;
	projattacking = false;

	offset = 0.f;
	flip_a = 1;
	flip_b = -1;
	changedir = 0.087;

		direction_p_a = 1.f;
		direction_p_b = 0.f;
}

void Boss::stateManager()
{
	if (boss_health < (75 * 0.01 * 300))
		spin = true;
	if (boss_health < (50 * 0.01 * 300)) // 50% hp
		tailAtk = true;
	if (boss_health < (25 * 0.01 * 300)) // 25% hp
	{
		DmgOverTime = true;
		magicImmunity = true;
	}
}

void Boss::dmgOvertime(double timeElapsed)
{
	if (DmgOverTime)
	{
		if (PlayerClass::get_instance()->get_health() > 20 && timeElapsed > bufferTime_DoT) // 20% hp of player
		{
			PlayerClass::get_instance()->healthSystem(true, false); // to make it -10 health every 7 second
			bufferTime_DoT = timeElapsed + 7.f;
		}
	}
}

void Boss::bossHealthUI()
{
	if (boss_health >= 0)
	{
		bossLife.heartCounter = boss_health / 10;

		if (bossLife.heartCounter == 30)
		{
			for (int i = 0; i < 30; i++)
				bossLife.boss_heart[i] = 2;
		}
		else
		{
			for (int i = 0; i < bossLife.heartCounter; i++)
			{
				bossLife.boss_heart[i] = 2;
			}
			for (int i = bossLife.heartCounter; i < 30; i++)
			{
				bossLife.boss_heart[i] = 0;
			}
		}
	}
}

void Boss::bossHealthSystem(bool laser)
{
	if (!laser)
	{
		this->boss_health -= 10;
		PlayerClass::get_instance()->energySystem();
	}
	else
	{
		this->boss_health -= 60;
	}
}

void Boss::burrowTeleportation(double timeElapsed)
{
	int burrowDirection = 0;
	int burrowRange = 0;
	
	if (burrow && timeElapsed > cooldown_Burrow && // 15 second cooldown
		!tailattacking && !spinning && !burrowing) // multiple actions should not happen at the same time and also burrowing not occured yet
	{
		burrowing = true;
		burrowDist = true;
	
		if (burrowDist == true)
		{
			burrowRange = rand() % 50 + 10; // to ensure teleport range is at least 10 distance
			burrowDist = false; // to prevent continuous update of burrowRange
			burrowDir = true;
		}
		if (burrowDir == true) 
		{
			burrowDirection = rand() % 2 + 1; // to decide the direction of boss teleporting
			burrowDir = false; // to prevent continuous update of burrowDirection
		}

		if (burrowDirection == 1 && position_m.x < 50) // Prevents the boss from moving in that direction if too near
		{ // Example : Boss cannot teleport leftwards if he is too near to the left border
			position_m.x += burrowRange;

			/*-------To prevent boss teleportation position to overlapse with the char position---*/
			if (position_m.x < PlayerClass::get_instance()->position_a.x + 6 &&
				position_m.x > PlayerClass::get_instance()->position_a.x - 1)
			{
				position_m.x = PlayerClass::get_instance()->position_a.x + 8;
			}
			else if (position_m.x > PlayerClass::get_instance()->position_a.x - 6 &&
				position_m.x < PlayerClass::get_instance()->position_a.x)
			{
				position_m.x = PlayerClass::get_instance()->position_a.x - 8;
			}
			/*------------------------------------------------------------------------------------*/

			if (position_m.x > 65) // Prevents the boss from escaping the boundary
				position_m.x = 55;
			
			cooldown_Burrow = timeElapsed + 15.f;
			burrowing = false;
		}
		else if (burrowDirection == 2 && position_m.x > -50)
		{
			position_m.x -= burrowRange;

			/*-------To prevent boss teleportation position to overlapse with the char position---*/
			if (position_m.x < PlayerClass::get_instance()->position_a.x + 6 &&
				position_m.x > PlayerClass::get_instance()->position_a.x - 1)
			{
				position_m.x = PlayerClass::get_instance()->position_a.x + 8;
			}
			else if (position_m.x > PlayerClass::get_instance()->position_a.x - 6 &&
				position_m.x < PlayerClass::get_instance()->position_a.x)
			{
				position_m.x = PlayerClass::get_instance()->position_a.x - 8;
			}
			/*------------------------------------------------------------------------------------*/

			if (position_m.x < -65)
				position_m.x = -55;

			cooldown_Burrow = timeElapsed + 15.f;
			burrowing = false;
 		}
	}
}

void Boss::spinAttack(double timeElapsed, bool block)
{
	if (timeElapsed > cooldown_Spin)
	{
		cooldown_Spin = timeElapsed + 8.f; // spin happens every 8 second PROVIDED if it dun overlapse with another action
		spinningDuration = timeElapsed + 4.f;
	}

	if (!burrowing && !tailattacking && spinningDuration > timeElapsed)
	{
		if (PlayerClass::get_instance()->PlayerHitBox.collide(Boss::get_instance()->EnemyHitBox) && (timeElapsed > bufferTime_iframe))
		{
			bufferTime_iframe = timeElapsed + 1.f;
			PlayerClass::get_instance()->healthSystem(block, true);
		}
	}
}

void Boss::tailAttack(double timeElapsed, bool block)
{
	if (tailAtk && !burrowing && !spinning)
	{
		if ((timeElapsed > bufferTime_tail) && (timeElapsed < bufferTime_tail + 3.f))			  // stalk for 3 seconds
			this->Boss_Tail.stalk();
		else if ((timeElapsed > bufferTime_tail + 3.5f) && (timeElapsed < bufferTime_tail + 6.f)) // 0.5 seconds of no movement, "locking on". strike for 2.5 seconds.
			this->Boss_Tail.strike(block);
		else if ((timeElapsed > bufferTime_tail + 6.f) && (timeElapsed < bufferTime_tail + 7.5f)) // 1.5s of retracting back into the ground.
			this->Boss_Tail.retract(block);
		else if (timeElapsed > bufferTime_tail + 7.5f)
			bufferTime_tail = timeElapsed + 1.f;												  // 1 second cd before cycling back again to stalk, strike, retract.
	}
}

void Boss::proj_attack(Vector3 pos, Vector3 dir, double elapsedTime)
{
	if (elapsedTime > bufferTime_projduration)
	{
		if (attackchoice == 3 && tailattacking)
		{
			if ((elapsedTime > bufferTime_tail + 3.5f) && (elapsedTime < bufferTime_tail + 7.5))
			{
				for (direction_p_a = -1.f; direction_p_a < 1.f; direction_p_a += 0.4f)
				{
					this->spit_.push_back(projectileBuilder::GenerateProjectile(3, pos, (dir + Vector3(0, 0, direction_p_a))));
				}

				bufferTime_projduration = elapsedTime + 1.f;
			}
		}
		else if (attackchoice == 2 && spinning)
		{
			if (elapsedTime > bufferTime_resetdir + 2.5f)
			{
				bufferTime_resetdir = elapsedTime + 5.f;
				direction_p_a = 1.f;
				direction_p_b = 0.f;
			}

			if (bufferTime_resetdir > elapsedTime)
			{
				if (direction_p_b < 0.f && direction_p_a > 0.f)
					flip_a = -1;
				if (direction_p_b < 0.f && direction_p_a < 0.f)
					flip_b = 1;
				if (direction_p_b > 0.f && direction_p_a < 0.f)
					flip_a = 1;
				if (direction_p_b > 0.f && direction_p_a > 0.f)
					flip_b = -1;

				direction_p_a += (flip_a * changedir);
				direction_p_b += (flip_b * changedir);

				if (direction_p_a > 1.f)
					direction_p_a = 1.f;
				else if (direction_p_a < -1.f)
					direction_p_a = -1.f;

				if (direction_p_b > 1.f)
					direction_p_b = 1.f;
				else if (direction_p_b < -1.f)
					direction_p_b = -1.f;

				dir.Set(direction_p_a, 0, direction_p_b);

				this->spit_.push_back(projectileBuilder::GenerateProjectile(3, pos, dir));

				bufferTime_projduration = elapsedTime + 0.1f;
			}
		}
		//else if (attackchoice == 4 && burrowing)
		//{
		//	this->spit_.push_back(projectileBuilder::GenerateProjectile(3, pos, dir));

		//	bufferTime_projduration = elapsedTime + 0.1f;
		//}
		else if (attackchoice == 1)
		{
			if (elapsedTime > bufferTime_projcd)
			{
				bufferTime_projcd = elapsedTime + 2.f;
				projtype = rand() % 4 + 1;
			}

			if (bufferTime_projcd - 1.75f > elapsedTime)
			{
				this->spit_.push_back(projectileBuilder::GenerateProjectile(projtype, pos, dir));

				bufferTime_projduration = elapsedTime + 0.05f;
			}
		}
	}
}

unsigned int Boss::get_action()
{
	//if (this->attackchoice == 4)
	//	return 1;
	if (this->attackchoice == 2)
		return 2;
	else if (this->attackchoice == 3)
		return 3;
	else
		return 0;
}

bool Boss::spin_delay_anim()
{
	if (spinning)
		return true;
	else
		return false;
}

void Boss::boss_attack(double elapsedTime, bool block)
{
	//if (burrow)
	//	random_choice = rand() % 4 + 1; // praise RNGesus
	if (tailAtk)
		random_choice = rand() % 3 + 1;
	else if (spin)
		random_choice = rand() % 2 + 1;
	else
		random_choice = 1;

	if (elapsedTime > bufferTime_attackchoice)
	{
		tailattacking = false;
		spinning = false;
		//burrowing = false;

		if (random_choice == 3)
		{
			tailattacking = true;
			bufferTime_attackchoice = elapsedTime + 12.f;
		}
		else if (random_choice == 2)
		{
			bufferTime_attackchoice = elapsedTime + 12.5f;
		}
		//else if (random_choice == 4)
		//{
		//	bufferTime_attackchoice = elapsedTime + 15.f;
		//}
		else if (random_choice == 1)
		{
			bufferTime_attackchoice = elapsedTime + 4.f;
		}

		attackchoice = random_choice;
	}
	else
	{
		if ((attackchoice == 2 || attackchoice == 3) && (elapsedTime > (bufferTime_attackchoice - 4.f)))
		{
			tailattacking = false;
			spinning = false;
			attackchoice = 1;
		}

		if (attackchoice == 1)
		{
			std::cout << "normal" << std::endl;
		}
		else if (attackchoice == 3)
		{
			std::cout << "tail" << std::endl;
			this->tailAttack(elapsedTime, true);
		}
		else if (attackchoice == 2)
		{
			std::cout << "spin" << std::endl;

			if (elapsedTime + 10.5f > bufferTime_attackchoice)
			{
				spinning = true;
				this->spinAttack(elapsedTime, false);
			}
			else
			{
			}
		}
		//else if (attackchoice == 4)
		//{
		//	std::cout << "burrow" << std::endl;
		//	this->burrowTeleportation(elapsedTime);
		//}
	}
}

void Boss::facingDirection()
{
	if ((this->position_m.x - PlayerClass::get_instance()->position_a.x) > 5)
		this->direction_m.x = -1;
	else if ((this->position_m.x - PlayerClass::get_instance()->position_a.x) < -5)
		this->direction_m.x = 1;

	this->Boss_Tail.set_direction();
}

void Boss::update(double timeElapsed, bool block)
{
	this->bossHealthUI();
	this->stateManager();
	this->dmgOvertime(timeElapsed);
	this->facingDirection();
	this->boss_attack(timeElapsed, block);

	if (Application::IsKeyPressed('V'))
	{
		if (boss_health > 0)
			boss_health -= 10;
	}
	if (Application::IsKeyPressed('C'))
	{
		if (boss_health < 300)
			boss_health += 10;
	}
}

void Boss::reset()
{
	this->boss_health = 300;
}