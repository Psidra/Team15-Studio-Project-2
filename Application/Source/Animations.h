#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "MatrixStack.h"
#include <string>

// Alexis

void AnimCheck(unsigned int num_anim, MS* modelStack, double* timeElapsed, const std::string& dagNode);

// I feel like an idiot, why did I even think doing that(below) was a good idea

//void AttackAnim(bool attack, MS* modelStack, double* timeElapsed, const std::string& dagNode);
//void WalkAnim(MS* modelStack, double* timeElapsed, const std::string& dagNode);
//void SlideAnim(bool trigger, MS* modelStack, double* timeElapsed, const std::string& dagNode);
//void GrabAnim(bool grab, MS* modelStack, double* timeElapsed, const std::string& dagNode);
//void InjectAnim(bool inject, MS* modelStack, double* timeElapsed, const std::string& dagNode);
//void DeathAnim(bool isdead, MS* modelStack, double* timeElapsed, const std::string& dagNode);
//void RollAnim(bool dodge, MS* modelStack, double* timeElapsed, const std::string& dagNode);
//void BlockAnim(bool block, MS* modelStack, double* timeElapsed, const std::string& dagNode);

// 1/2 Mutant

void AnimCheck_H_Mutant(unsigned int num_anim, MS* modelStack, double* timeElapsed, const std::string& dagNode);

//void IdleAnim(MS* modelStack, double* timeElapsed, const std::string& dagNode);
//void Transform(bool inject, MS* modelStack, double* timeElapsed, const std::string& dagNode);

// Mutant

void AnimCheck_Mutant(unsigned int num_anim, MS* modelStack, double* timeElapsed, const std::string& dagNode);

//void IdleAnim_M(MS* modelStack, double* timeElapsed, const std::string& dagNode);
//void AttackAnim_M(bool attack_M, MS* modelStack, double* timeElapsed, const std::string& dagNode);
//void WalkAnim_M(MS* modelStack, double* timeElapsed, const std::string& dagNode);
//void DeathAnim_M(bool isdead_M, MS* modelStack, double* timeElapsed, const std::string& dagNode);

// Boss
// oh god no

void AnimCheck_Boss(unsigned int num_anim, MS* modelStack, double* timeElapsed, const std::string& dagNode);

#endif // ANIMATIONS_H