#include <GLFW/glfw3.h>
#include "Camera3.h"
#include "Application.h"
#include "Mtx44.h"
#include "StudioProject2_Scene1.h"

extern GLFWwindow* m_window;

/******************************************************************************/
/*!
\brief
Default constructor
*/
/******************************************************************************/
Camera3::Camera3()
{
}

/******************************************************************************/
/*!
\brief
Destructor
*/
/******************************************************************************/
Camera3::~Camera3()
{
}

/******************************************************************************/
/*!
\brief
Initialize camera

\param pos - position of camera
\param target - where the camera is looking at
\param up - up vector of camera
*/
/******************************************************************************/
void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = pos;
	this->target = target;
	this->up = up;

	this->position_bc = pos;

	Vector3 view = (target - position).Normalized();
	Vector3 right = (view.Cross(up)).Normalized();
	this->up = right.Cross(view);

	bounds = 192;
}

/******************************************************************************/
/*!
\brief
Reset the camera settings
*/
/******************************************************************************/
void Camera3::Reset()
{
}

/******************************************************************************/
/*!
\brief
To be called every frame. Camera will get user inputs and update its position and orientation

\param dt - frame time
*/
/******************************************************************************/
void Camera3::Update(double dt)
{
	Vector3 view = target - position;
	view.Normalize();
	Vector3 right = view.Cross(up);
	right.Normalize();

	position_bc = position;

	if (Application::IsKeyPressed('A'))
	{
		position_bc = position_bc - right;

		if (position_bc.x < bounds && position_bc.x > -bounds)
		{
			position.x = position.x - right.x;
			target.x = position.x + view.x;
		}
		else
		{
			position = position;
			target = position + view;
		}
		
		//if (position_bc.y < bounds && position_bc.y > 1)
		//{
		//	position.y = position.y - right.y;
		//	target.y = position.y + view.y;
		//}
		//else
		//{
		//	position = position;
		//	target = position + view;
		//}
		
		if (position_bc.z < bounds && position_bc.z > -bounds)
		{
			position.z = position.z - right.z;
			target.z = position.z + view.z;
		}
		else
		{
			position = position;
			target = position + view;
		}
	}
	else if (Application::IsKeyPressed('D'))
	{
		position_bc = position_bc + right;

		if (position_bc.x < bounds && position_bc.x > -bounds)
		{
			position.x = position.x + right.x;
			target.x = position.x + view.x;
		}
		else
		{
			position = position;
			target = position + view;
		}

		//if (position_bc.y < bounds && position_bc.y > 1)
		//{
		//	position.y = position.y + right.y;
		//	target.y = position.y + view.y;
		//}
		//else
		//{
		//	position = position;
		//	target = position + view;
		//}

		if (position_bc.z < bounds && position_bc.z > -bounds)
		{
			position.z = position.z + right.z;
			target.z = position.z + view.z;
		}
		else
		{
			position = position;
			target = position + view;
		}
	}
	else if (Application::IsKeyPressed('W'))
	{
		position_bc = position + (view * 40.f * dt);

		if (position_bc.x < bounds && position_bc.x > -bounds)
		{
			position.x = position.x + (view.x * 40.f * dt);
			target.x = position.x + view.x;
		}
		else
		{
			position = position;
			target = position + view;
		}

		//if (position_bc.y < bounds && position_bc.y > 1)
		//{
		//	position.y = position.y + (view.y * 40.f * dt);
		//	target.y = position.y + view.y;
		//}
		//else
		//{
		//	position = position;
		//	target = position + view;
		//}

		if (position_bc.z < bounds && position_bc.z > -bounds)
		{
			position.z = position.z + (view.z * 40.f * dt);
			target.z = position.z + view.z;
		}
		else
		{
			position = position;
			target = position + view;
		}
	}
	else if (Application::IsKeyPressed('S'))
	{
		position_bc = position - (view * 40.f * dt);

		if (position_bc.x < bounds && position_bc.x > -bounds)
		{
			position.x = position.x - (view.x * 40.f * dt);
			target.x = position.x + view.x;
		}
		else
		{
			position = position;
			target = position + view;
		}

		//if (position_bc.y < bounds && position_bc.y > 1)
		//{
		//	position.y = position.y - (view.y * 40.f * dt);
		//	target.y = position.y + view.y;
		//}
		//else
		//{
		//	position = position;
		//	target = position + view;
		//}

		if (position_bc.z < bounds && position_bc.z > -bounds)
		{
			position.z = position.z - (view.z * 40.f * dt);
			target.z = position.z + view.z;
		}
		else
		{
			position = position;
			target = position + view;
		}
	}
	else if (Application::IsKeyPressed('N'))
	{
		position = position + up;
		target = position + view;
	}
	else if (Application::IsKeyPressed('M'))
	{
		position = position - up;
		target = position + view;
	}
	else if (Application::IsKeyPressed('R'))
	{
		position.y = 20;
		target = position + view;
	}

	// rat movement

	//if (MouseControl)
	//{
	//	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//	glfwGetCursorPos(m_window, &MouseX, &MouseY);
	//	offsetX = 400 - MouseX;
	//	offsetY = MouseY - 300;

	//	if (offsetX > 0)
	//	{
	//		float yaw = (float)(offsetX * dt * sens);
	//		Mtx44 rotation;
	//		rotation.SetToRotation(yaw, 0, 1, 0);
	//		view = rotation * view;
	//		target = position + view;
	//		up = rotation * up;
	//		glfwSetCursorPos(m_window, 400, 300);
	//	}
	//	if (offsetX < 0)
	//	{
	//		float yaw = (float)(offsetX * dt * sens);
	//		Mtx44 rotation;
	//		rotation.SetToRotation(yaw, 0, 1, 0);
	//		view = rotation * view;
	//		target = position + view;
	//		up = rotation * up;
	//		glfwSetCursorPos(m_window, 400, 300);
	//	}

	//	if (offsetY > 0)
	//	{
	//		float pitch = (float)(-offsetY * dt * sens);
	//		Mtx44 rotation;
	//		right.y = 0;
	//		rotation.SetToRotation(pitch, right.x, right.y, right.z);
	//		up = right.Cross(view).Normalized();
	//		view = rotation * view;
	//		target = position + view;
	//		glfwSetCursorPos(m_window, 400, 300);
	//	}
	//	if (offsetY < 0)
	//	{
	//		float pitch = (float)(-offsetY * dt * sens);
	//		Mtx44 rotation;
	//		right.y = 0;
	//		rotation.SetToRotation(pitch, right.x, right.y, right.z);
	//		up = right.Cross(view).Normalized();
	//		view = rotation * view;
	//		target = position + view;
	//		glfwSetCursorPos(m_window, 400, 300);
	//	}
	//}
	//else
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		if (Application::IsKeyPressed(VK_LEFT))
		{
			float yaw = 50.f * dt;
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			up = rotation * up;

			view = rotation * view;
			target = position + view;
		}
		else if (Application::IsKeyPressed(VK_RIGHT))
		{
			float yaw = -50.f * dt;
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			up = rotation * up;

			view = rotation * view;
			target = position + view;
		}
		else if (Application::IsKeyPressed(VK_UP))
		{
			float pitch = 40.f * dt;
			Mtx44 rotation;
			rotation.SetToRotation(pitch, right.x, right.y, right.z);
			up = right.Cross(view);

			view = rotation * view;
			target = position + view;
		}
		else if (Application::IsKeyPressed(VK_DOWN))
		{
			float pitch = -40.f * dt;
			Mtx44 rotation;
			rotation.SetToRotation(pitch, right.x, right.y, right.z);
			up = right.Cross(view);

			view = rotation * view;
			target = position + view;
		}
	}
}