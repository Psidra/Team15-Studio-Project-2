#include <GLFW/glfw3.h>
#include "Camera4_SideScroller.h"
#include "Application.h"
#include "Mtx44.h"
#include "StudioProject2_Scene1.h"

extern GLFWwindow* m_window;
extern bool MouseControl;

/******************************************************************************/
/*!
\brief
Default constructor
*/
/******************************************************************************/
Camera4::Camera4()
{
}

/******************************************************************************/
/*!
\brief
Destructor
*/
/******************************************************************************/
Camera4::~Camera4()
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
void Camera4::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = pos;
	this->target = target;
	this->up = up;

	Vector3 view = (target - position).Normalized();
	Vector3 right = (view.Cross(up)).Normalized();
	this->up = right.Cross(view);

}

/******************************************************************************/
/*!
\brief
Reset the camera settings
*/
/******************************************************************************/
void Camera4::Reset()
{
}

/******************************************************************************/
/*!
\brief
To be called every frame. Camera will get user inputs and update its position and orientation

\param dt - frame time
*/
/******************************************************************************/
void Camera4::Update(double dt, float posx, float posy)
{
	Vector3 view = (target - position).Normalized();
	Vector3 right = (view.Cross(up)).Normalized();

	position.x = posx;
	position.y = posy;
	target = position + view;
}
	// rat movement

	/*if (MouseControl == true)
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwGetCursorPos(m_window, &MouseX, &MouseY);
		offsetX = 400 - MouseX;
		offsetY = MouseY - 300;

		if (offsetX > 0)
		{
			float yaw = (float)(offsetX * dt * sens);
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			view = rotation * view;
			target = position + view;
			up = rotation * up;
			glfwSetCursorPos(m_window, 400, 300);
		}
		if (offsetX < 0)
		{
			float yaw = (float)(offsetX * dt * sens);
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			view = rotation * view;
			target = position + view;
			up = rotation * up;
			glfwSetCursorPos(m_window, 400, 300);
		}

		if (offsetY > 0)
		{
			float pitch = (float)(-offsetY * dt * sens);
			Mtx44 rotation;
			right.y = 0;
			rotation.SetToRotation(pitch, right.x, right.y, right.z);
			up = right.Cross(view).Normalized();
			view = rotation * view;
			target = position + view;
			glfwSetCursorPos(m_window, 400, 300);
		}
		if (offsetY < 0)
		{
			float pitch = (float)(-offsetY * dt * sens);
			Mtx44 rotation;
			right.y = 0;
			rotation.SetToRotation(pitch, right.x, right.y, right.z);
			up = right.Cross(view).Normalized();
			view = rotation * view;
			target = position + view;
			glfwSetCursorPos(m_window, 400, 300);
		}
	}
	else
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
}*/