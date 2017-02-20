#include "Projectile.h"
#include <cmath>

unsigned int Projectile::displacement()
{
	return (sqrt((this->defaultPosition_.x - this->position_.x)*(this->defaultPosition_.x - this->position_.x) +
		(this->defaultPosition_.y - this->position_.y)*(this->defaultPosition_.y - this->position_.y) +
		(this->defaultPosition_.z - this->position_.z)*(this->defaultPosition_.z - this->position_.z)));
}