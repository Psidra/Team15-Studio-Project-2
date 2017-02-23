#include "Boss.h"
#include "PlayerClass.h"
#include "Application.h"
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
			PlayerClass::get_instance()->healthSystem(true);
			bufferTime_DoT = timeElapsed + 7.f;
		}
	}
}

void Boss::bossHealthUI()
{
	if (boss_health > 0)
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

void Boss::bossHealthSystem()
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
	
	if (burrow && timeElapsed > bufferTime_Burrow && // 15 second cooldown
		!tailattacking && !spinning) // multiple actions should not happen at the same time
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

			if (position_m.x > 65) // Prevents the boss from escaping the boundary
				position_m.x = 55;

			bufferTime_Burrow = timeElapsed + 15.f;
			burrowing = false;
		}
		else if (burrowDirection == 2 && position_m.x > -50)
		{
			position_m.x -= burrowRange;

			if (position_m.x < -65)
				position_m.x = -55;

			bufferTime_Burrow = timeElapsed + 15.f;
			burrowing = false;
 		}
		else if (burrowDirection == 3 && position_m.z < 50)
		{
			position_m.z += burrowRange;

			if (position_m.z > 65)
				position_m.z = 55;

			bufferTime_Burrow = timeElapsed + 15.f;
			burrowing = false;
		}
		else if (burrowDirection == 4 && position_m.z > -50)
		{
			position_m.z -= burrowRange;

			if (position_m.z < -65)
				position_m.z = -55;

			bufferTime_Burrow = timeElapsed + 15.f;
			burrowing = false;
		}
	}
}

void spinAttack(double timeElapsed)
{

}

void tailAttack(double timeElapsed)
{

}