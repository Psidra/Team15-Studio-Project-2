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
		case 6:
			LoadAtom("ATOM//walk2.atom", modelStack, timeElapsed, dagNode);
			break;
		default:
			LoadAtom("ATOM//attack.atom", modelStack, &zero, dagNode);
			break;
		}
}

// 1/2 Mutant

void IdleAnim(MS* modelStack, double* timeElapsed, const std::string& dagNode)
{

}

void Transform(bool inject, MS* modelStack, double* timeElapsed, const std::string& dagNode)
{

}

// Mutant

void AnimCheck_Mutant(unsigned int num_anim, MS* modelStack, double* timeElapsed, const std::string& dagNode)
{
	double zero = 0;
	switch (num_anim) {
	case 20:
		LoadAtom("ATOM//FullMutant_IdleMovement.atom", modelStack, timeElapsed, dagNode);
		break;
	default:
		LoadAtom("ATOM//FullMutant_IdleMovement.atom", modelStack, timeElapsed, dagNode);
		break;
	}
}

// Boss

// oh god no