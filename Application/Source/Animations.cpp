#include "Animations.h"
#include "LoadATOM.h"

// Alexis

void AttackAnim(bool attack, MS* modelStack, double* timeElapsed, const std::string& dagNode)
{
	double zero = 0;
	if (attack)
		LoadAtom("ATOM//attack.atom", modelStack, timeElapsed, dagNode);
	else if (!attack)
		LoadAtom("ATOM//attack.atom", modelStack, &zero, dagNode);
}

void WalkAnim(MS* modelStack, double* timeElapsed, const std::string& dagNode)
{

}

void SlideAnim(bool trigger, MS* modelStack, double* timeElapsed, const std::string& dagNode)
{

}

void GrabAnim(bool grab, MS* modelStack, double* timeElapsed, const std::string& dagNode)
{

}

void InjectAnim(bool inject, MS* modelStack, double* timeElapsed, const std::string& dagNode)
{

}

void DeathAnim(bool isdead, MS* modelStack, double* timeElapsed, const std::string& dagNode)
{

}

void RollAnim(bool dodge, MS* modelStack, double* timeElapsed, const std::string& dagNode)
{

}

void BlockAnim(bool block, MS* modelStack, double* timeElapsed, const std::string& dagNode)
{

}

// 1/2 Mutant

void IdleAnim(MS* modelStack, double* timeElapsed, const std::string& dagNode)
{

}

void Transform(bool inject, MS* modelStack, double* timeElapsed, const std::string& dagNode)
{

}

// Mutant

void IdleAnim_M(MS* modelStack, double* timeElapsed, const std::string& dagNode)
{
	LoadAtom("ATOM//FullMutant_IdleMovement.atom", modelStack, timeElapsed, dagNode);
}

void AttackAnim_M(bool attack_M, MS* modelStack, double* timeElapsed, const std::string& dagNode)
{

}

void WalkAnim_M(MS* modelStack, double* timeElapsed, const std::string& dagNode)
{

}

void DeathAnim_M(bool isdead_M, MS* modelStack, double* timeElapsed, const std::string& dagNode)
{

}

// Boss

// oh god no