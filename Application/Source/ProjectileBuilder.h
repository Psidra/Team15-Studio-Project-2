#ifndef PROJECTILEBUILDER_H
#define PROJECTILEBUILDER_H

#include "Projectile.h"

class projectileBuilder
{
public:
	static Projectile* GenerateProjectile(unsigned int projType, Vector3 pos, Vector3 dir);
};
#endif // PROJECTILEBUILDER_H