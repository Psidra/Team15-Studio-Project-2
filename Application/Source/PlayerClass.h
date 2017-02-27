#ifndef PLAYERCLASS_H
#define PLAYERCLASS_H

#include "BoundingBox.h"

class PlayerClass
{
public:
	static PlayerClass* get_instance()
	{
		if (!instance)
			instance = new PlayerClass();
		else
			return instance;
	}

	int get_health();
	int get_energy();

	void healthUI();
	void manaUI();
	void energySystem(double timeElapsed);
	void init();
	void facingDirection();
	void healthSystem(bool Block , bool bossTagged);
	void healthRegeneration(double timeElapsed);
	void bossFightFacingDirection();
	void timeSpent(float dt);
	void restartLevel();
	void restartGame();
	void projectileShield(double timeElapsed);
	void laserBeam(double timeElapsed);
	void spellUI(double timeElapsed);
	
	struct PlayerLife
	{
		float a_heart[10];
		float a_blankheart[10];
		int heartCounter;

		float a_energy[10];
		float a_blankenergy[10];
		int energyCounter;
	};

	struct SpellHUD
	{
		float projShieldReady;
		float projShieldNotReady;

		float laserReady;
		float laserNotReady;
	};

	BoundingBox PlayerHitBox;
	Vector3 position_a;
	Vector3 laserSize;
	Vector3 laserTranslate;
	PlayerLife Hearts;
	SpellHUD spellHUD;
	int a_LookingDirection;
	int hm_Saved;
	int fm_Killed;
	double timeSpend;

private:
	PlayerClass(): _health(100), _energy(100), a_LookingDirection(90), hm_Saved(0), fm_Killed(0), timeSpend(0.0f)
	, energyProjShield(50), energyLaser(100), laserActive(false), projSheildActive(false) {};
	static PlayerClass* instance;

	double bufferTime_Laser;
	double bufferTime_ProjShield;
	double bufferTime_healthRegen;
	unsigned int energyProjShield;
	unsigned int energyLaser;
	unsigned int _health;
	unsigned int healthStorage;
	int hmSavedStorage;
	int fmKilledStorage;
	int energyStorage;
	unsigned int _energy;
	bool isDead();
	bool laserActive;
	bool projSheildActive;
};

#endif //PLAYERCLASS_H