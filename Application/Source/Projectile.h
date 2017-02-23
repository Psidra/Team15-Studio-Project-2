#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <cmath>
#include "vector3.h"
#include "BoundingBox.h"

class Projectile
{
public:
	Projectile() {};
	Projectile(unsigned int projType, Vector3 pos, Vector3 dir) : projType_(projType), defaultPosition_(pos), direction_(dir) {
		this->x_distance = -1;
		position_ = pos;
	};
	~Projectile() {};

	unsigned int projType_; //different bullet rendering
	float projSpeed = 1.0f; // anything beyond 10 activates sanic speedz. Actually its projSpeed x 60 for m/s ._.
	Vector3 position_; //stores current position
	Vector3 direction_; //direction of the bullet, taken from model or camera
	Vector3 defaultPosition_; //used to find displacement
	BoundingBox projHitBox_; //used to detect if bullet hits anything

	unsigned int displacement(); //find distance between two points, defaultPosition and position

	void init_proj(float distbetweenthem);
	void edit_passed_angle();
	float get_passed_angle();

private:
	const float g = 0.0163f;	//  real life gravity too stronk. ((9.8 / 60) / 10).
	const float pi = 3.14159f;  // close enough, no point going so far in.
	float x_distance;
	float init_angle;
	float passed_angle;
};

#endif // PROJECTILE_H