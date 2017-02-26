#include "Tail.h"
#include "PlayerClass.h"

void Tail::stalk()
{
	this->position_t.x = PlayerClass::get_instance()->position_a.x;
	this->position_t.y = PlayerClass::get_instance()->position_a.y - 10.f;
	this->position_t.z = PlayerClass::get_instance()->position_a.z;
}

void Tail::strike(bool block)
{
	this->position_t.y += 10.f;
	this->TailHitBox.translate(0, 10.f, 0);
	
	if (PlayerClass::get_instance()->PlayerHitBox.collide(this->TailHitBox))
		PlayerClass::get_instance()->healthSystem(block, true);
}

void Tail::retract(bool block)
{
	this->position_t.y -= 20.f;
	this->TailHitBox.translate(0, -20.f, 0);

	if (PlayerClass::get_instance()->PlayerHitBox.collide(this->TailHitBox))
		PlayerClass::get_instance()->healthSystem(block, true);
}