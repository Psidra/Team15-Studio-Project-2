#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "MatrixStack.h"
#include <string>

// Alexis

void AnimCheck(unsigned int num_anim, MS* modelStack, double* timeElapsed, const std::string& dagNode);

// 1/2 Mutant

void AnimCheck_H_Mutant(unsigned int num_anim, MS* modelStack, double* timeElapsed, const std::string& dagNode);

// Mutant

void AnimCheck_Mutant(unsigned int num_anim, MS* modelStack, double* timeElapsed, const std::string& dagNode);

// Boss
// oh god no

//shitty bat flying animation (BACKGROUND *INTERACTION)
void Batanim(bool batfly, MS* modelStack, double* timeElapsed, const std::string& dagNode);

void AnimCheck_Boss(unsigned int num_anim, MS* modelStack, double* timeElapsed, const std::string& dagNode);

#endif // ANIMATIONS_H