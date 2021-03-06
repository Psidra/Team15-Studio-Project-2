#include "Tail.h"
#include "Boss.h"
#include "PlayerClass.h"

void Tail::stalk()
{
	this->position_t.Set(PlayerClass::get_instance()->position_a.x, PlayerClass::get_instance()->position_a.y - 200.f, PlayerClass::get_instance()->position_a.z);
	this->TailHitBox.translate(this->position_t.x, this->position_t.y, this->position_t.z);
}

void Tail::strike(bool block)
{
	this->position_t.y = 1.f;
	this->TailHitBox.scale(1.25f, 5.f, 30.f);
	this->TailHitBox.translate(this->position_t.x, this->position_t.y, this->position_t.z);

	if (PlayerClass::get_instance()->PlayerHitBox.collide(this->TailHitBox))
		PlayerClass::get_instance()->healthSystem(block, true);
}

void Tail::retract(bool block)
{
	this->position_t.y = -200.f;
	this->TailHitBox.scale(1.25f, 5.f, 30.f);
	this->TailHitBox.translate(this->position_t.x, this->position_t.y, this->position_t.z);

	if (PlayerClass::get_instance()->PlayerHitBox.collide(this->TailHitBox))
		PlayerClass::get_instance()->healthSystem(block, true);
}

void Tail::set_direction()
{
	this->direction_t = (PlayerClass::get_instance()->position_a - this->position_t).Normalized();
}