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
	get_instance()->position_m = Vector3(0, 5, 0);
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
}

void Boss::stateManager()
{
	if (boss_health < (75 * 0.01 * 300)) // 75% hp  [75/100 * 300] note: Multiply is cheaper than divide
		spin = true;
	if (boss_health < (50 * 0.01 * 300)) // 50% hp
		tailAtk = true;
	if (boss_health < (25 * 0.01 * 300)) // 25% hp
		burrow = true;
	if (boss_health < (10 * 0.01 * 300)) // 10% hp
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

void Boss::bossHealthSystem(double timeElapsed)
{
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

void Boss::burrowTeleportation(double timeElapsed)
{
	int burrowDirection = 0;
	int burrowRange = 0;
	srand(time(NULL));
	
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
			burrowDirection = rand() % 4 + 1; // to decide the direction of boss teleporting
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
		else if (burrowDirection == 3 && position_m.z < 50)
		{
			position_m.z += burrowRange;

			/*-------To prevent boss teleportation position to overlapse with the char position---*/
			if (position_m.z < PlayerClass::get_instance()->position_a.z + 6 &&
				position_m.z > PlayerClass::get_instance()->position_a.z - 1)
			{
				position_m.z = PlayerClass::get_instance()->position_a.z + 8;
			}
			else if (position_m.z > PlayerClass::get_instance()->position_a.z - 6 &&
				position_m.z < PlayerClass::get_instance()->position_a.z)
			{
				position_m.z = PlayerClass::get_instance()->position_a.z - 8;
			}
			/*------------------------------------------------------------------------------------*/

			if (position_m.z > 65)
				position_m.z = 55;

			cooldown_Burrow = timeElapsed + 15.f;
			burrowing = false;
		}
		else if (burrowDirection == 4 && position_m.z > -50)
		{
			position_m.z -= burrowRange;

			/*-------To prevent boss teleportation position to overlapse with the char position---*/
			if (position_m.z < PlayerClass::get_instance()->position_a.z + 6 &&
				position_m.z > PlayerClass::get_instance()->position_a.z - 1)
			{
				position_m.z = PlayerClass::get_instance()->position_a.z + 8;
			}
			else if (position_m.z > PlayerClass::get_instance()->position_a.z - 6 &&
				position_m.z < PlayerClass::get_instance()->position_a.z)
			{
				position_m.z = PlayerClass::get_instance()->position_a.z - 8;
			}
			/*------------------------------------------------------------------------------------*/

			if (position_m.z < -65)
				position_m.z = -55;

			cooldown_Burrow = timeElapsed + 15.f; // burrow happens every 15 second PROVIDED if it dun overlapse with another action
			burrowing = false;
		}
	}
}

void Boss::spinAttack(double timeElapsed , bool block)
{
	if (spin)
	{
		if (timeElapsed > cooldown_Spin && !burrowing
			&& !tailattacking && !spinning)
		{
			spinning = true;
			spinningDuration = timeElapsed + 5.f;
			if (EnemyHitBox.collide(PlayerClass::get_instance()->PlayerHitBox))
			{
				PlayerClass::get_instance()->healthSystem(block, true);
			}

			if (timeElapsed > spinningDuration) // every spin is 5 seconds
			{
				spinning = false;
				cooldown_Spin = timeElapsed + 8.f; // spin happens every 8 second PROVIDED if it dun overlapse with another action
			}
		}
	}
}

void Boss::tailAttack(double timeElapsed, bool block)
{
	if (tailAtk && !burrowing && !spinning)
	{
		tailattacking = true;

		if ((timeElapsed > bufferTime_tail) && (timeElapsed < bufferTime_tail + 3.f))			  // stalk for 3 seconds
			this->Boss_Tail.stalk();
		else if ((timeElapsed > bufferTime_tail + 3.5f) && (timeElapsed < bufferTime_tail + 6.f)) // 0.5 seconds of no movement, "locking on". strike for 2.5 seconds.
			this->Boss_Tail.strike(block);
		else if ((timeElapsed > bufferTime_tail + 6.f) && (timeElapsed < bufferTime_tail + 7.5f)) // 1.5s of retracting back into the ground.
			this->Boss_Tail.retract(block);
		else if	(timeElapsed > bufferTime_tail + 7.5f)
		{
			bufferTime_tail = timeElapsed + 1.f;												  // 1 second cd before cycling back again to stalk, strike, retract.
			tailattacking = false;
		}
	}
}

void Boss::proj_attack(unsigned int projType, Vector3 pos, Vector3 dir, double elapsedTime)
{
	//if (bufferTime_attackpattern > elapsedTime)
	//{
	//if (!burrowing && !spinning && !tailattacking)
	//{
	//		if (attack_pattern == 1)
	//		{
	//			for (unsigned int loop = 0; loop < 10; loop++)
	//			{
	//				this->spit_.push_back(projectileBuilder::GenerateProjectile(projType, pos, dir));
	//			}

	//			bufferTime_attackpattern = 0;
	//			projattacking = false;
	//		}
	//		else if (attack_pattern == 2)
	//		{
	//			for (unsigned int loop = 0; loop < 10; loop++)
	//			{
	//				this->spit_.push_back(projectileBuilder::GenerateProjectile(projType, pos, dir));
	//			}
	//		}
	//	}
	//	else
	//	{
	//		srand(time(NULL));
	//		attack_pattern = rand() % 3 + 1; // if we even have time for 3 different patterns

	//		bufferTime_attackpattern = elapsedTime + 8.f;
	//		projattacking = true;
	//	}
	//}
	//else
	//{
	//	projattacking = false;
	//}s
}

unsigned int Boss::get_action()
{
	if (burrowing)
		return 1;
	else if (spinning)
		return 2;
	else if (tailattacking)
		return 3;
	else if (projattacking)
		return 4;
	else
		return 0;
}