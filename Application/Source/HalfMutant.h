#ifndef HALFMUTANT_H
#define HALFMUTANT_H

#include "PlayerClass.h"

class HalfMutant
{
public:
	HalfMutant();
	~HalfMutant();

	void init();
	void transformation();
	void movement(double dt);
	Vector3 position_hm;
	Vector3 size_hm;
	Vector3 size_human;

	int hm_LookingDirection;

private:
	bool transformed; 
	float positionStorageX1;
	float positionStorageX2;
	bool charNear;
};

#endif