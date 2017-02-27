#include "BoundingBox.h"

void BoundingBox::loadBB(const std::string &file_path)
{
	std::ifstream fileStream(file_path, std::ios::binary);

	Vector3 vLowerLeftFront = (-999999999.0f, -999999999.0f, 999999999.0f); // one more digit and the world implodes on itself I think.
	Vector3 vUpperRightBack = (999999999.0f, 999999999.0f, -999999999.0f);  // ...or something similar to that extent. (Max value is 2 bil.)
																			// Update: Okay, nothing happened. Still, don't add another digit.
	while (!fileStream.eof())
	{
		char buf[256];
		fileStream.getline(buf, 256);
		if (strncmp("v ", buf, 2) == 0)
		{
			Vector3 vertex;
			sscanf_s((buf + 2), "%f%f%f", &vertex.x, &vertex.y, &vertex.z);

			// Update lower-left-front corner of BoundingBox
			vLowerLeftFront.x = min(vLowerLeftFront.x, vertex.x);
			vLowerLeftFront.y = min(vLowerLeftFront.y, vertex.y);
			vLowerLeftFront.z = max(vLowerLeftFront.z, vertex.z);
			// Update upper-right-back corner of BoundingBox
			vUpperRightBack.x = max(vUpperRightBack.x, vertex.x);
			vUpperRightBack.y = max(vUpperRightBack.y, vertex.y);
			vUpperRightBack.z = min(vUpperRightBack.z, vertex.z);
		}
	}
	fileStream.close();

	dmax_ = vUpperRightBack;
	dmin_ = vLowerLeftFront;
	max_ = vUpperRightBack;
	min_ = vLowerLeftFront;
}

void BoundingBox::translate(float x, float y, float z)
{
	this->max_.x += x;
	this->max_.y += y;
	this->max_.z += z;

	this->min_.x += x;
	this->min_.y += y;
	this->min_.z += z;
}

void BoundingBox::scale(float x, float y, float z)
{
	this->max_.x *= x;
	this->max_.y *= y;
	this->max_.z *= z;

	this->min_.x *= x;
	this->min_.y *= y;
	this->min_.z *= z;
}

bool BoundingBox::collide(BoundingBox &b)
{
	return (b.min_.x <= BoundingBox::max_.x && b.max_.x >= BoundingBox::min_.x) &&
		(b.min_.y <= BoundingBox::max_.y && b.max_.y >= BoundingBox::min_.y) &&
		(b.min_.z >= BoundingBox::max_.z && b.max_.z <= BoundingBox::min_.z);
}

void BoundingBox::resetBB()
{
	max_ = dmax_;
	min_ = dmin_;
}

bool BoundingBox::higherthan(BoundingBox &b)
{
	return (b.max_.y <= BoundingBox::min_.y);
}

void BoundingBox::setto(float x, float y, float z) // ain't workin
{
	this->max_.x = x;
	this->max_.y = y;
	this->max_.z = z;

	this->min_.x = x;
	this->min_.y = y;
	this->min_.z = z;
}