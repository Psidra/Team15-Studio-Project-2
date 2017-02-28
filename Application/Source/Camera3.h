#ifndef CAMERA3_H
#define CAMERA3_H

#include "Camera.h"

class Camera3 : public Camera
{
public:
	Camera3();
	~Camera3();

	Vector3 initPosition;
	Vector3 initTarget;
	Vector3 initUp;

	double MouseX = 0;
	double MouseY = 0;

	double offsetX = 0;
	double offsetY = 0; //HELLO DARKNESS MY OLD FRIEND 
	float sens = 7; //IVE COME TO SEE U AGAIN

	void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	void Reset();
	void Update(double dt);

	int bounds;
};

#endif