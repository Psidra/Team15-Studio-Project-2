#include "Projectile.h"
#include <cmath>

unsigned int Projectile::displacement()
{
	return (sqrt((this->defaultPosition_.x - this->position_.x)*(this->defaultPosition_.x - this->position_.x) +
		(this->defaultPosition_.y - this->position_.y)*(this->defaultPosition_.y - this->position_.y) +
		(this->defaultPosition_.z - this->position_.z)*(this->defaultPosition_.z - this->position_.z)));
}

void Projectile::init_proj(float distbetweenthem)									 // It's probably possible(and easier) to put this in constructor tbh
{
	if (x_distance == -1)
	{
		this->x_distance = (distbetweenthem / 60.f);			// div by 60... because... I can't explain well. "Reasons"?
		this->init_angle = ((pi - (asin(x_distance * g))) / 2); // pi - angle cause other direction (rad)
		this->passed_angle = init_angle;
	}
}

void Projectile::edit_passed_angle()
{
	float rate_of_change = (1/x_distance * 3.5f) * (-g * pow(cos((pi / 180) * init_angle), 2)); // jesus this was annoying to type
	passed_angle += rate_of_change;															// rising cause A S T C, we're shooting from the other side.
}

void Projectile::boss_edit_passed_angle()
{
	float rate_of_change = (1 / x_distance * 3.5f) * (-g * pow(cos((pi / 180) * init_angle), 2));
	passed_angle += rate_of_change;
}


float Projectile::get_passed_angle()
{
	return passed_angle;
}