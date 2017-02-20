#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "vector3.h"
#include "BoundingBox.h"

class Projectile
{
public:
	Projectile() {};
	Projectile(unsigned int projType, Vector3 pos, Vector3 dir) : projType_(projType), defaultPosition_(pos), direction_(dir){
		position_ = pos;
	};
	~Projectile() {};

	unsigned int projType_; //different bullet rendering
	float projSpeed = 5.0f; // anything beyond 10 activates sanic speedz
	Vector3 position_; //stores current position
	Vector3 direction_; //direction of the bullet, taken from model or camera
	Vector3 defaultPosition_; //used to find displacement
	BoundingBox projHitBox_; //used to detect if bullet hits anything

	//possible collision check

	unsigned int displacement(); //find distance between two points, defaultPosition and position
};

#endif // PROJECTILE_H