#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#include <fstream>
#include "Vector3.h"

class BoundingBox
{
public:
	Vector3 max_;
	Vector3 min_;
	Vector3 dmax_;
	Vector3 dmin_;

	void loadBB(const std::string &file_path);
	void resetBB();
	void translate(float x, float y, float z);
	void scale(float x, float y, float z);
	bool collide(BoundingBox &b);
};

#endif // !BOUNDINGBOX_H