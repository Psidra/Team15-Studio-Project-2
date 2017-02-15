#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "Vertex.h"
#include "Mesh.h"

#include <vector>

/******************************************************************************/
/*!
		Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/
class MeshBuilder
{
public:
    static Mesh* GenerateAxis(const std::string &meshName);
    static Mesh* GenerateQuad(const std::string &meshName, Color color);
    static Mesh* GenerateCube(const std::string &meshName, Color color);
    static Mesh* GenerateCircle(const std::string &meshName, Color color, unsigned numSlices = 10, float radius = 1.0f);
    static Mesh* GenerateRing(const std::string &meshName, Color color, unsigned numSlices = 10, float radius = 1.0f, float thickness = 0.8f);
    static Mesh* GenerateSphere(const std::string &meshName, Color color, unsigned numStacks = 18, unsigned numSlices = 36, float radius = 1.0f);
    static Mesh* GenerateHemisphere(const std::string &meshName, Color color, unsigned numStacks = 9, unsigned numSlices = 36, float radius = 1.0f);
    static Mesh* GenerateCylinder(const std::string &meshName, Color color, unsigned numStacks = 10, unsigned numSlices = 36, float radius = 1.0f, float height = 1.0f);
    static Mesh* GenerateStarCuboid(const std::string &meshName, Color color, unsigned numPoints = 5, float radius = 1.0f, float innerRadius = 0.5f, float height = 1.0f);
	static Mesh* GenerateCone(const std::string &meshName, Color color, unsigned int numSlices = 36, float radius = 1.f, int height = 3, bool isCovered = true);
	static Mesh* GenerateOBJ(const std::string &meshName, const std::string &file_path);
	static Mesh* GenerateText(const std::string &meshName, unsigned numRow, unsigned numCol);
	static Mesh* GenerateBB(const std::string &meshName, Vector3 max, Vector3 min);
};

#endif