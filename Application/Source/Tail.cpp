#include "Tail.h"
#include "Boss.h"
#include "PlayerClass.h"

void Tail::stalk()
{
	this->position_t.Set(PlayerClass::get_instance()->position_a.x, PlayerClass::get_instance()->position_a.y - 20.f, PlayerClass::get_instance()->position_a.z);
	this->TailHitBox.setto(this->position_t.x, this->position_t.y, this->position_t.z);
}

void Tail::strike(bool block)
{
	this->position_t.y = 5.f;
	this->TailHitBox.setto(this->position_t.x, this->position_t.y, this->position_t.z);
	
	if (PlayerClass::get_instance()->PlayerHitBox.collide(this->TailHitBox))
		PlayerClass::get_instance()->healthSystem(block, true);
}

void Tail::retract(bool block)
{
	this->position_t.y = -20.f;
	this->TailHitBox.setto(this->position_t.x, this->position_t.y, this->position_t.z);

	if (PlayerClass::get_instance()->PlayerHitBox.collide(this->TailHitBox))
		PlayerClass::get_instance()->healthSystem(block, true);
}

void Tail::set_direction()
{
	this->direction_t = (PlayerClass::get_instance()->position_a - this->direction_t).Normalized();
}