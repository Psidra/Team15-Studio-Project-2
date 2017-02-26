#ifndef TAIL_H
#define TAIL_H

#include "BoundingBox.h"

class Tail
{
public:
	static Tail* get_instance()
	{
		if (!instance)
			instance = new Tail();
		else
			return instance;
	}

	void stalk();
	void lockon();
	void strike();

	BoundingBox TailHitBox;
	Vector3 position_t;

private:
	Tail() {};
	static Tail* instance;

};

#endif //TAIL_H