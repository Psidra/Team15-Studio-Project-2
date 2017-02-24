#include "HalfMutant.h"
#include "Application.h"
#include "PlayerClass.h"
#include "GLFW\glfw3.h"

HalfMutant::HalfMutant()
{

}

HalfMutant::~HalfMutant()
{

}

void HalfMutant::init()
{
	hm_LookingDirection = 90.f;
	positionStorageX1 = position_hm.x + 10; 
	positionStorageX2 = position_hm.x - 10;	
	charNear = false;
	transformed = false;
}

void HalfMutant::movement(double dt)
{
	if (charNear == true || transformed == true) // doesnt move when transformed to human or character is near
	{
		if (position_hm.x < PlayerClass::get_instance()->position_a.x)
			hm_LookingDirection = 90.f;
		else
			hm_LookingDirection = -90.f;
	}
	else // idle movement when character not near
	{
		static float movementDir = 1.f;
		position_hm.x += (float)(5.f * dt * movementDir);
		if ((position_hm.x > positionStorageX1 && movementDir != -1.f) || 
			(position_hm.x < positionStorageX2 && movementDir != 1.f))
		{
			movementDir *= -1;
		}

		if (movementDir == 1)
			hm_LookingDirection = 90.f;
		else
			hm_LookingDirection = -90.f;
	}
}

void HalfMutant::transformation()
{
	int distBetweenThem = position_hm.x - PlayerClass::get_instance()->position_a.x;

	if ((distBetweenThem < 3) && (distBetweenThem > -3) && transformed == false)
		charNear = true;
	else
		charNear = false;

	int Controller = glfwJoystickPresent(GLFW_JOYSTICK_1);

	if (Controller == 1)
	{
		int xboxcontrols;
		const unsigned char *xbox = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &xboxcontrols);
		if (charNear == true && (Application::IsKeyPressed('F') || GLFW_PRESS == xbox[5]))
		{
			transformed = true;
			charNear = false;
			saveCount = true;
			size_hm = Vector3(0.1, 0.1, 0.1);
			size_human = Vector3(1, 1, 1);
		}
	}
	else
	{
		if (charNear == true && Application::IsKeyPressed('F'))
		{
			transformed = true;
			charNear = false;
			saveCount = true;
			size_hm = Vector3(0.1, 0.1, 0.1);
			size_human = Vector3(1, 1, 1);
		}
	}

	if (saveCount == true)
	{
		PlayerClass::get_instance()->hm_Saved++;
		saveCount = false;
	}
}

bool HalfMutant::get_near()
{
	return charNear;
}