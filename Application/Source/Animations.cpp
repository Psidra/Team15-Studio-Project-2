#include "Animations.h"
#include "LoadATOM.h"

// Alexis

void AnimCheck(unsigned int num_anim, MS* modelStack, double* timeElapsed, const std::string& dagNode)
{
		double zero = 0;
		switch (num_anim) {
		case 0:
			LoadAtom("ATOM//attack.atom", modelStack, timeElapsed, dagNode);
			break;
		case 7:
			LoadAtom("ATOM//duckroll.atom", modelStack, timeElapsed, dagNode);
			break;
		case 8:
			LoadAtom("ATOM//block.atom", modelStack, timeElapsed, dagNode);
			break;
		case 9:
			LoadAtom("ATOM//grab.atom", modelStack, timeElapsed, dagNode);
			break;
		case 6:
			LoadAtom("ATOM//walk2.atom", modelStack, timeElapsed, dagNode);
			break;
		default:
			LoadAtom("ATOM//attack.atom", modelStack, &zero, dagNode);
			break;
		}
}

// 1/2 Mutant

void AnimCheck_H_Mutant(unsigned int num_anim, MS* modelStack, double* timeElapsed, const std::string& dagNode)
{
	double zero = 0;
	switch (num_anim) {
	case 10:
		LoadAtom("ATOM//halfwalk2.atom", modelStack, timeElapsed, dagNode);
		break;
	case 11:
		LoadAtom("ATOM//halfidle.atom", modelStack, timeElapsed, dagNode);
		break;
	}
}

// Mutant

void AnimCheck_Mutant(unsigned int num_anim, MS* modelStack, double* timeElapsed, const std::string& dagNode)
{
	double zero = 0;
	switch (num_anim) {
	case 20:
		LoadAtom("ATOM//FullMutant_IdleMovement.atom", modelStack, timeElapsed, dagNode);
		break;
	case 21:
		LoadAtom("ATOM//Updated_FullMutant_WalkCycle.atom", modelStack, timeElapsed, dagNode);
		break;
	case 22:
		LoadAtom("ATOM//FullMutant_MeleeAttack.atom", modelStack, timeElapsed, dagNode);
		break;
	case 23:
		LoadAtom("ATOM//FullMutant_SpitAttack.atom", modelStack, timeElapsed, dagNode);
		break;
	default:
		LoadAtom("ATOM//FullMutant_IdleMovement.atom", modelStack, timeElapsed, dagNode);
		break;
	}
}

// Boss

// oh god no

//shit bat anim
void Batanim(bool batfly, MS* modelStack, double* timeElapsed, const std::string& dagNode)
{
	LoadAtom("ATOM//bat.atom", modelStack, timeElapsed, dagNode);
}