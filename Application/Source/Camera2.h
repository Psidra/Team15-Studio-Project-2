#ifndef CAMERA2_H
#define CAMERA2_H

#include "Camera.h"

class Camera2 : public Camera
{
public:
    Camera2();
    ~Camera2();

    void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
    void Reset();
    void Update(double dt);
};

#endif