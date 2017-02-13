#include "Camera2.h"
#include "Application.h"
#include "Mtx44.h"

/******************************************************************************/
/*!
\brief
Default constructor
*/
/******************************************************************************/
Camera2::Camera2()
{
}

/******************************************************************************/
/*!
\brief
Destructor
*/
/******************************************************************************/
Camera2::~Camera2()
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
void Camera2::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
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
void Camera2::Reset()
{
}

/******************************************************************************/
/*!
\brief
To be called every frame. Camera will get user inputs and update its position and orientation

\param dt - frame time
*/
/******************************************************************************/
void Camera2::Update(double dt)
{
    Vector3 view = target - position;
    view.Normalize();
    Vector3 right = view.Cross(up);
    right.Normalize();

    if (Application::IsKeyPressed('A'))
    {
        float yaw = -50.f * dt;
        Mtx44 rotation;
        rotation.SetToRotation(yaw, 0, 1, 0);
        position = rotation * position;
        up = rotation * up;
    }
    else if (Application::IsKeyPressed('D'))
    {
        float yaw = 50.f * dt;
        Mtx44 rotation;
        rotation.SetToRotation(yaw, 0, 1, 0);
        position = rotation * position;
        up = rotation * up;
    }
    else if (Application::IsKeyPressed('W'))
    {
        float pitch = -40.f * dt;
        Mtx44 rotation;
        rotation.SetToRotation(pitch, right.x, right.y, right.z);
        position = rotation * position;
        up = right.Cross(view);
    }
    else if (Application::IsKeyPressed('S'))
    {
        float pitch = 40.f * dt;
        Mtx44 rotation;
        rotation.SetToRotation(pitch, right.x, right.y, right.z);
        position = rotation * position;
        up = right.Cross(view);
    }
    else if (Application::IsKeyPressed('N'))
    {
        position += 40.f * view * dt;
        up = right.Cross(view);
    }
    else if (Application::IsKeyPressed('M'))
    {
        position -= 40.f * view * dt;
        up = right.Cross(view);
    }
}