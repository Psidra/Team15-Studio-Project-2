#ifndef Camera4_H
#define Camera4_H

#include "Camera.h"

class Camera4 : public Camera
{
public:
	Camera4();
	~Camera4();

	Vector3 initPosition;
	Vector3 initTarget;
	Vector3 initUp;

	double MouseX = 0;
	double MouseY = 0;

	double offsetX = 0;
	double offsetY = 0;
	float sens = 7;

	void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	void Reset();
	void Update(double dt, float posx, float posy);
	void UpdateTopdown(double dt, float posx, float posz);
	void UpdateUnlockedCam(double dt);
	void UpdateUnlockedCam2(double dt);
	void UpdateUnlockedCam3(double dt);

	int bounds;
};

#endif