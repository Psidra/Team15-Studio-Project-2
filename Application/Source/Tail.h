#ifndef TAIL_H
#define TAIL_H

#include "BoundingBox.h"

class Tail
{
public:
	Tail() : position_t(0, -10, 0) {};
	~Tail() {};

	void stalk();
	void strike(bool block);
	void retract(bool block);

	BoundingBox TailHitBox;
	Vector3 position_t;

private:

};

#endif //TAIL_H