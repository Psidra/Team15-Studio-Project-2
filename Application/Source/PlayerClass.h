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

	void edit_health(int HP);
	unsigned int get_health();

	void add_energy(int EP);
	unsigned int get_energy();

	struct Coordinates
	{
		float posX;
		float posY;
		float posZ;
	};

	Coordinates Coord;

private:
	PlayerClass() {};
	static PlayerClass* instance;

	BoundingBox PlayerHitBox;

	unsigned int _health;
	unsigned int _energy;
	bool _dead;
};

#endif //PLAYERCLASS_H