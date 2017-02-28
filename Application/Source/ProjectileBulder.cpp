#include "projectileBuilder.h"
#include "MeshBuilder.h"

Projectile* projectileBuilder::GenerateProjectile(unsigned int projType, Vector3 pos, Vector3 dir)
{
	if (projType == 1) // Mutant Projectile
	{
		Projectile* spit = new Projectile(projType, pos, dir);
		return spit;
	}
	if (projType == 2) // Mutant Projectile
	{
		Projectile* spit = new Projectile(projType, pos, dir);
		return spit;
	}
	if (projType == 3) // Mutant Projectile
	{
		Projectile* spit = new Projectile(projType, pos, dir);
		return spit;
	}
	if (projType == 4) // Mutant Projectile
	{
		Projectile* spit = new Projectile(projType, pos, dir);
		return spit;
	}
}