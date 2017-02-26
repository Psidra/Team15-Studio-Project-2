#include "Tail.h"
#include "PlayerClass.h"

void Tail::stalk()
{
	this->position_t = PlayerClass::get_instance()->position_a;
}

void Tail::strike()
{
	this->position_t.y += 10.f;
	this->TailHitBox.translate(0, 0, 10.f);
	
	if (PlayerClass::get_instance()->PlayerHitBox.collide(this->TailHitBox))
	{

	}
}