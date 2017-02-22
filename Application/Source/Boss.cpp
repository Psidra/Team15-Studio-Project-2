#include "Boss.h"
#include "PlayerClass.h"
#include "Application.h"

Boss* Boss::instance;

unsigned int Boss::get_health()
{
	return boss_health;
}

void Boss::bossInit()
{
	get_instance()->position_m = Vector3(-70, 0, 0);
	spin = false;
	tailAtk = false;
	burrow = false;
	DmgOverTime = false;
	magicImmunity = false;
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

}

void spinAttack(double timeElapsed)
{

}

void tailAttack(double timeElapsed)
{

}