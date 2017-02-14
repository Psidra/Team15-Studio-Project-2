#ifndef PLAYERCLASS_H
#define PLAYERCLASS_H

class PlayerClass
{
public:
	static PlayerClass* get_instance()
	{
		if (!instance)
			instance = new PlayerClass;
		else
			return instance;
	}

	void add_health(int HP);
	unsigned int get_health();

	void add_energy(int EP);
	unsigned int get_energy();

private:
	PlayerClass() {};
	static PlayerClass* instance;

	unsigned int _health;
	unsigned int _energy;
};

#endif //PLAYERCLASS_H