#include "MeshBuilder.h"
#include <GL\glew.h>
#include "MyMath.h"
#include "LoadOBJ.h"

/******************************************************************************/
/*!
\brief
Generate the vertices of a reference Axes; Use red for x-axis, green for y-axis, blue for z-axis
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - x-axis should start at -lengthX / 2 and end at lengthX / 2
\param lengthY - y-axis should start at -lengthY / 2 and end at lengthY / 2
\param lengthZ - z-axis should start at -lengthZ / 2 and end at lengthZ / 2

\return Pointer to mesh storing VBO/IBO of reference axes
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateAxis(const std::string &meshName)
{
    std::vector<Vertex> vertex_buffer_data;
    std::vector<unsigned> index_buffer_data;
    Vertex v;

    v.pos.Set(-1000.0f, 0.0f, 0.0f);    v.color.Set(1, 0, 0);  vertex_buffer_data.push_back(v);
    v.pos.Set(1000.0f, 0.0f, 0.0f);     v.color.Set(1, 0, 0);  vertex_buffer_data.push_back(v);
    v.pos.Set(0.0f, -1000.0f, 0.0f);    v.color.Set(0, 1, 0);  vertex_buffer_data.push_back(v);
    v.pos.Set(0.0f, 1000.0f, 0.0f);     v.color.Set(0, 1, 0);  vertex_buffer_data.push_back(v);
    v.pos.Set(0.0f, 0.0f, -1000.0f);    v.color.Set(0, 0, 1);  vertex_buffer_data.push_back(v);
    v.pos.Set(0.0f, 0.0f, 1000.0f);     v.color.Set(0, 0, 1);  vertex_buffer_data.push_back(v);

    for (unsigned i = 0; i < 6; ++i)
        index_buffer_data.push_back(i);

    Mesh *mesh = new Mesh(meshName);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

    mesh->indexSize = index_buffer_data.size();
    mesh->mode = Mesh::DRAW_LINES;

    return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a quad; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of quad
\param lengthY - height of quad

\return Pointer to mesh storing VBO/IBO of quad
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateQuad(const std::string &meshName, Color color)
{
    std::vector<Vertex> vertex_buffer_data;
    std::vector<unsigned> index_buffer_data;
    Vertex v;

	v.pos.Set(0.5f, 0.5f, 0.0f);	v.texCoord.Set(1, 1);	v.color = color;	v.normal.Set(0.f, 1.f, 0.f);    vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, 0.5f, 0.0f);	v.texCoord.Set(0, 1);	v.color = color;	v.normal.Set(0.f, 1.f, 0.f);    vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, -0.5f, 0.0f);	v.texCoord.Set(0, 0);	v.color = color;	v.normal.Set(0.f, 1.f, 0.f);    vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, 0.5f, 0.0f);	v.texCoord.Set(1, 1);	v.color = color;	v.normal.Set(0.f, 1.f, 0.f);    vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, -0.5f, 0.0f);	v.texCoord.Set(0, 0);	v.color = color;	v.normal.Set(0.f, 1.f, 0.f);    vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, -0.5f, 0.0f);	v.texCoord.Set(1, 0);	v.color = color;	v.normal.Set(0.f, 1.f, 0.f);    vertex_buffer_data.push_back(v);

    for (unsigned i = 0; i < 6; ++i)
    {
        index_buffer_data.push_back(0);
        index_buffer_data.push_back(i);
    }

    Mesh *mesh = new Mesh(meshName);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

    mesh->indexSize = index_buffer_data.size();
    mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

    return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a cube; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of cube
\param lengthY - height of cube
\param lengthZ - depth of cube

\return Pointer to mesh storing VBO/IBO of cube
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateCube(const std::string &meshName, Color color)
{
    std::vector<Vertex> vertex_buffer_data;
    std::vector<unsigned> index_buffer_data;
    Vertex v;

	// Front
	v.pos.Set(0.5f, 0.5f, 0.5f);    v.color = color;  v.normal.Set(0.f, 1.f, 0.f);	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, 0.5f, 0.5f);   v.color = color;  v.normal.Set(0.f, 1.f, 0.f);	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, -0.5f, 0.5f);  v.color = color;  v.normal.Set(0.f, 1.f, 0.f);	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, 0.5f, 0.5f);    v.color = color;  v.normal.Set(0.f, 1.f, 0.f);	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, -0.5f, 0.5f);  v.color = color;  v.normal.Set(0.f, 1.f, 0.f);	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, -0.5f, 0.5f);   v.color = color;  v.normal.Set(0.f, 1.f, 0.f);	vertex_buffer_data.push_back(v);
   
	// Right
	v.pos.Set(0.5f, 0.5f, 0.5f);    v.color = color;  vertex_buffer_data.push_back(v);
    v.pos.Set(0.5f, -0.5f, 0.5f);   v.color = color;  vertex_buffer_data.push_back(v);
    v.pos.Set(0.5f, 0.5f, -0.5f);   v.color = color;  vertex_buffer_data.push_back(v);
    v.pos.Set(0.5f, 0.5f, -0.5f);   v.color = color;  vertex_buffer_data.push_back(v);
    v.pos.Set(0.5f, -0.5f, 0.5f);   v.color = color;  vertex_buffer_data.push_back(v);
    v.pos.Set(0.5f, -0.5f, -0.5f);  v.color = color;  vertex_buffer_data.push_back(v);

	// Back
    v.pos.Set(0.5f, 0.5f, -0.5f);   v.color = color;  vertex_buffer_data.push_back(v);
    v.pos.Set(0.5f, -0.5f, -0.5f);  v.color = color;  vertex_buffer_data.push_back(v);
    v.pos.Set(-0.5f, 0.5f, -0.5f);  v.color = color;  vertex_buffer_data.push_back(v);
    v.pos.Set(-0.5f, 0.5f, -0.5f);  v.color = color;  vertex_buffer_data.push_back(v);
    v.pos.Set(0.5f, -0.5f, -0.5f);  v.color = color;  vertex_buffer_data.push_back(v);
    v.pos.Set(-0.5f, -0.5f, -0.5f); v.color = color;  vertex_buffer_data.push_back(v);

	// Left
    v.pos.Set(-0.5f, 0.5f, -0.5f);  v.color = color;  vertex_buffer_data.push_back(v);
    v.pos.Set(-0.5f, -0.5f, -0.5f); v.color = color;  vertex_buffer_data.push_back(v);
    v.pos.Set(-0.5f, 0.5f, 0.5f);   v.color = color;  vertex_buffer_data.push_back(v);
    v.pos.Set(-0.5f, 0.5f, 0.5f);   v.color = color;  vertex_buffer_data.push_back(v);
    v.pos.Set(-0.5f, -0.5f, -0.5f); v.color = color;  vertex_buffer_data.push_back(v);
    v.pos.Set(-0.5f, -0.5f, 0.5f);  v.color = color;  vertex_buffer_data.push_back(v);

	// Top
    v.pos.Set(-0.5f, 0.5f, 0.5f);   v.color = color;  vertex_buffer_data.push_back(v);
    v.pos.Set(0.5f, 0.5f, 0.5f);    v.color = color;  vertex_buffer_data.push_back(v);
    v.pos.Set(0.5f, 0.5f, -0.5f);   v.color = color;  vertex_buffer_data.push_back(v);
    v.pos.Set(-0.5f, 0.5f, 0.5f);   v.color = color;  vertex_buffer_data.push_back(v);
    v.pos.Set(0.5f, 0.5f, -0.5f);   v.color = color;  vertex_buffer_data.push_back(v);
    v.pos.Set(-0.5f, 0.5f, -0.5f);  v.color = color;  vertex_buffer_data.push_back(v);

	// Bottom
    v.pos.Set(-0.5f, -0.5f, -0.5f); v.color = color;  vertex_buffer_data.push_back(v);
    v.pos.Set(0.5f, -0.5f, 0.5f);   v.color = color;  vertex_buffer_data.push_back(v);
    v.pos.Set(-0.5f, -0.5f, 0.5f);  v.color = color;  vertex_buffer_data.push_back(v);
    v.pos.Set(-0.5f, -0.5f, -0.5f); v.color = color;  vertex_buffer_data.push_back(v);
    v.pos.Set(0.5f, -0.5f, -0.5f);  v.color = color;  vertex_buffer_data.push_back(v);
    v.pos.Set(0.5f, -0.5f, 0.5f);   v.color = color;  vertex_buffer_data.push_back(v);

    for (unsigned i = 0; i < 36; ++i)
        index_buffer_data.push_back(i);

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();

	return mesh;
}


Mesh* MeshBuilder::GenerateCircle(const std::string &meshName, Color color, unsigned numSlices, float radius)
{
    std::vector<Vertex> vertex_buffer_data;
    std::vector<unsigned> index_buffer_data;
    Vertex v;

    float anglePerSlice = 360.0f / numSlices;

    v.pos.Set(0.0f, 0.0f, 0.0f);
    v.color = color;
    vertex_buffer_data.push_back(v);

    for (unsigned slice = 0; slice < numSlices + 1; ++slice)
    {
        float theta = slice * anglePerSlice;
        v.pos.Set(radius * cos(Math::DegreeToRadian(theta)), 0.0f, radius * sin(Math::DegreeToRadian(theta)));
        v.color = color;
        vertex_buffer_data.push_back(v);
    }

    for (unsigned i = 0; i < numSlices + 1; ++i)
    {
        index_buffer_data.push_back(i + 1);
        index_buffer_data.push_back(0);
    }

    Mesh *mesh = new Mesh(meshName);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

    mesh->indexSize = index_buffer_data.size();
    mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

    return mesh;
}

Mesh* MeshBuilder::GenerateRing(const std::string &meshName, Color color, unsigned numSlices, float radius, float thickness)
{
    std::vector<Vertex> vertex_buffer_data;
    std::vector<unsigned> index_buffer_data;
    Vertex v;

    float anglePerSlice = 360.0f / numSlices;

    for (unsigned slice = 0; slice < numSlices + 1; ++slice)
    {
        float theta = slice * anglePerSlice;
        v.pos.Set(radius * cos(Math::DegreeToRadian(theta)), 0.0f, radius * sin(Math::DegreeToRadian(theta)));
        v.color = color;
        vertex_buffer_data.push_back(v);

        v.pos.Set(radius * thickness * cos(Math::DegreeToRadian(theta)), 0.0f, radius * thickness * sin(Math::DegreeToRadian(theta)));
        v.color = color;
        vertex_buffer_data.push_back(v);

    }

    for (unsigned i = 0; i < (numSlices + 1) * 2; ++i)
        index_buffer_data.push_back(i);

    Mesh *mesh = new Mesh(meshName);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

    mesh->indexSize = index_buffer_data.size();
    mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

    return mesh;
}

Mesh* MeshBuilder::GenerateSphere(const std::string &meshName, Color color, unsigned numStacks, unsigned numSlices, float radius)
{
    std::vector<Vertex> vertex_buffer_data;
    std::vector<unsigned> index_buffer_data;
    Vertex v;

    float anglePerStack = 180.0f / numStacks;
    float anglePerSlice = 360.0f / numSlices;

    for (unsigned stack = 0; stack < numStacks + 1; ++stack)
    {
        float phi = -90 + stack * anglePerStack;
        for (unsigned slice = 0; slice < numSlices + 1; ++slice)
        {
            float theta = slice * anglePerSlice;
            v.pos.Set(radius * cos(Math::DegreeToRadian(phi)) * cos(Math::DegreeToRadian(theta)), radius * sin(Math::DegreeToRadian(phi)), radius * cos(Math::DegreeToRadian(phi)) * sin(Math::DegreeToRadian(theta)));
            v.color = color;
            v.normal.Set(cos(Math::DegreeToRadian(phi)) * cos(Math::DegreeToRadian(theta)), sin(Math::DegreeToRadian(phi)), cos(Math::DegreeToRadian(phi)) * sin(Math::DegreeToRadian(theta)));
            vertex_buffer_data.push_back(v);
        }
    }

    for (unsigned stack = 0; stack < numStacks; ++stack)
    {
        for (unsigned slice = 0; slice < numSlices + 1; ++slice)
        {
            index_buffer_data.push_back(stack * (numSlices + 1) + slice);
            index_buffer_data.push_back((stack + 1) * (numSlices + 1) + slice);
        }
    }

    Mesh *mesh = new Mesh(meshName);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

    mesh->indexSize = index_buffer_data.size();
    mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

    return mesh;
}

Mesh* MeshBuilder::GenerateHemisphere(const std::string &meshName, Color color, unsigned numStacks, unsigned numSlices, float radius)
{
    std::vector<Vertex> vertex_buffer_data;
    std::vector<unsigned> index_buffer_data;
    Vertex v;

    float anglePerStack = 90.0f / numStacks;
    float anglePerSlice = 360.0f / numSlices;

    v.pos.Set(0.0f, 0.0f, 0.0f);
    v.color = color;
    vertex_buffer_data.push_back(v);

    for (unsigned slice = 0; slice < numSlices + 1; ++slice)
    {
        float theta = slice * anglePerSlice;
        v.pos.Set(radius * cos(Math::DegreeToRadian(theta)), 0.0f, radius * sin(Math::DegreeToRadian(theta)));
        v.color = color;
        vertex_buffer_data.push_back(v);
    }

    for (unsigned stack = 1; stack < numStacks + 1; ++stack)
    {
        float phi = stack * anglePerStack;
        for (unsigned slice = 0; slice < numSlices + 1; ++slice)
        {
            float theta = slice * anglePerSlice;
            v.pos.Set(radius * cos(Math::DegreeToRadian(phi)) * cos(Math::DegreeToRadian(theta)), radius * sin(Math::DegreeToRadian(phi)), radius * cos(Math::DegreeToRadian(phi)) * sin(Math::DegreeToRadian(theta)));
            v.color = color;
            vertex_buffer_data.push_back(v);
        }
    }

    for (unsigned i = 0; i < numSlices + 1; ++i)
    {
        index_buffer_data.push_back(0);
        index_buffer_data.push_back(i + 1);
    }

    for (unsigned stack = 0; stack < numStacks; ++stack)
    {
        for (unsigned slice = 0; slice < numSlices + 1; ++slice)
        {
            index_buffer_data.push_back(stack * (numSlices + 1) + slice + 1);
            index_buffer_data.push_back((stack + 1) * (numSlices + 1) + slice + 1);
        }
    }

    Mesh *mesh = new Mesh(meshName);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

    mesh->indexSize = index_buffer_data.size();
    mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

    return mesh;
}

Mesh* MeshBuilder::GenerateCylinder(const std::string &meshName, Color color, unsigned numStacks, unsigned numSlices, float radius, float height)
{
    std::vector<Vertex> vertex_buffer_data;
    std::vector<unsigned> index_buffer_data;
    Vertex v;

    float stackHeight = height / numStacks;
    float anglePerSlice = 360.0f / numSlices;

    v.pos.Set(0.0f, -height * 0.5f, 0.0f);
    v.color = color;
    vertex_buffer_data.push_back(v);

    for (unsigned slice = 0; slice < numSlices + 1; ++slice)
    {
        float theta = slice * anglePerSlice;
        v.pos.Set(radius * cos(Math::DegreeToRadian(theta)), -height * 0.5f, radius * sin(Math::DegreeToRadian(theta)));
        v.color = color;
        vertex_buffer_data.push_back(v);
    }

    for (unsigned stack = 0; stack < numStacks; ++stack)
    {
        for (unsigned slice = 0; slice < numSlices + 1; ++slice)
        {
            float theta = slice * anglePerSlice;
            v.pos.Set(radius * cos(Math::DegreeToRadian(theta)), -height * 0.5f + (stack + 1) * stackHeight, radius * sin(Math::DegreeToRadian(theta)));
            v.color = color;
            vertex_buffer_data.push_back(v);
        }
    }

    v.pos.Set(0.0f, height * 0.5f, 0.0f);
    v.color = color;
    vertex_buffer_data.push_back(v);


    for (unsigned i = 0; i < numSlices + 1; ++i)
    {
        index_buffer_data.push_back(0);
        index_buffer_data.push_back(i + 1);
    }

    for (unsigned stack = 0; stack < numStacks + 1; ++stack)
    {
        if (stack < numStacks)
        {
            for (unsigned slice = 0; slice < numSlices + 1; ++slice)
            {
                index_buffer_data.push_back(stack * (numSlices + 1) + slice + 1);
                index_buffer_data.push_back((stack + 1) * (numSlices + 1) + slice + 1);
            }
        }
        else
        {
            for (unsigned slice = 0; slice < numSlices + 1; ++slice)
            {
                index_buffer_data.push_back(stack * (numSlices + 1) + slice + 1);
                index_buffer_data.push_back((numSlices + 1) * (numStacks + 1) + 1);
            }
        }
    }

    Mesh *mesh = new Mesh(meshName);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

    mesh->indexSize = index_buffer_data.size();
    mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

    return mesh;
}

Mesh* MeshBuilder::GenerateStarCuboid(const std::string &meshName, Color color, unsigned numPoints, float radius, float innerRadius, float height)
{
    std::vector<Vertex> vertex_buffer_data;
    std::vector<unsigned> index_buffer_data;
    Vertex v;

    float anglePerPoint = 360.f / numPoints;
    float anglePerInnerPoint = anglePerPoint / 2.f;

    v.pos.Set(0.0f, -height * 0.5f, 0.0f);
    v.color = color;
    vertex_buffer_data.push_back(v);

    for (unsigned point = 0; point < numPoints; ++point)
    {
        float theta = point * anglePerPoint;

        v.pos.Set(radius * cos(Math::DegreeToRadian(theta)), -height * 0.5f, radius * sin(Math::DegreeToRadian(theta)));
        v.color = color;
        vertex_buffer_data.push_back(v);

        v.pos.Set(innerRadius * cos(Math::DegreeToRadian(theta + anglePerInnerPoint)), -height * 0.5f, innerRadius * sin(Math::DegreeToRadian(theta + anglePerInnerPoint)));
        v.color = color;
        vertex_buffer_data.push_back(v);
    }

    for (unsigned point = 0; point < numPoints; ++point)
    {
        float theta = point * anglePerPoint;
        float phi = point * (anglePerPoint + anglePerInnerPoint);

        v.pos.Set(radius * cos(Math::DegreeToRadian(theta)), height * 0.5f, radius * sin(Math::DegreeToRadian(theta)));
        v.color = color;
        vertex_buffer_data.push_back(v);

        v.pos.Set(innerRadius * cos(Math::DegreeToRadian(theta + anglePerInnerPoint)), height * 0.5f, innerRadius * sin(Math::DegreeToRadian(theta + anglePerInnerPoint)));
        v.color = color;
        vertex_buffer_data.push_back(v);
    }

    v.pos.Set(0.0f, height * 0.5f, 0.0f);
    v.color = color;
    vertex_buffer_data.push_back(v);

    for (unsigned i = 0; i < numPoints * 2; ++i)
    {
        index_buffer_data.push_back(0);
        index_buffer_data.push_back(i + 1);
    }

    index_buffer_data.push_back(1);

    for (unsigned i = numPoints * 2 + 1; i < numPoints * 4 + 1; ++i)
    {
        index_buffer_data.push_back(i);
        index_buffer_data.push_back(i - (numPoints * 2 - 1));
    }

    for (unsigned i = numPoints * 2 + 1; i < numPoints * 6; ++i)
    {
        index_buffer_data.push_back(numPoints * 4 + 1);
        index_buffer_data.push_back(i);
    }

    Mesh *mesh = new Mesh(meshName);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

    mesh->indexSize = index_buffer_data.size();
    mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

    return mesh;
}

Mesh* MeshBuilder::GenerateCone(const std::string &meshName, Color color, unsigned int numSlices, float radius, int height, bool isCovered)
{
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	Vertex v;

	float anglePerSlice = 360.0f / numSlices;

	for (int slices = 0; slices < numSlices + 1; ++slices)
	{
		float theta = slices * anglePerSlice;
		v.pos.Set(
			radius * cos(Math::DegreeToRadian(theta)), //x
			-height / 2,									   //y
			radius * sin(Math::DegreeToRadian(theta))  //z
			);
		v.color = color;
		vertex_buffer_data.push_back(v);

		v.pos.Set(0, height / 2, 0);
		v.color = color;
		vertex_buffer_data.push_back(v);
	}
	if (isCovered == true)
	{
		for (int slices = 0; slices < numSlices + 1; ++slices)
		{
			v.pos.Set(0, -height / 2, 0);
			v.color = color;
			vertex_buffer_data.push_back(v);

			float theta = slices * anglePerSlice;
			v.pos.Set(
				radius * cos(Math::DegreeToRadian(theta)), //x
				-height / 2,							   //y
				radius * sin(Math::DegreeToRadian(theta))  //z
				);
			v.color = color;
			vertex_buffer_data.push_back(v);
		}
	}
	for (int slice = 0; slice < (numSlices * 5); ++slice)
	{
		index_buffer_data.push_back(slice);
	}

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateOBJ(const std::string &meshName, const std::string &file_path)
{
	//Read vertices, texcoords & normals from OBJ
	std::vector<Position> vertices;
	std::vector<TexCoord> uvs;
	std::vector<Vector3> normals;
	bool success = LoadOBJ(file_path.c_str(), vertices, uvs, normals);
	if (!success)
		return NULL;
	//Index the vertices, texcoords & normals properly
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);


	//Create the mesh and call glBindBuffer, glBufferData
	//Use triangle list and remember to set index size

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}

Mesh* MeshBuilder::GenerateText(const std::string &meshName, unsigned numRow, unsigned numCol)
{
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	Vertex v;

	float width = 1.f / numCol;
	float height = 1.f / numRow;
	for (unsigned row = 0; row < numRow; ++row)
	{
		for (unsigned col = 0; col < numCol; ++col)
		{
			v.pos.Set(-0.5f, 0.5f, 0.0f);	v.texCoord.Set(col * width, (16 - row) * height);				vertex_buffer_data.push_back(v);
			v.pos.Set(0.5f, 0.5f, 0.0f);	v.texCoord.Set((col + 1) * width, (16 - row) * height);		vertex_buffer_data.push_back(v);
			v.pos.Set(0.5f, -0.5f, 0.0f);	v.texCoord.Set((col + 1) * width, (16 - row - 1) * height);	vertex_buffer_data.push_back(v);
			v.pos.Set(-0.5f, -0.5f, 0.0f);	v.texCoord.Set(col * width, (16 - row - 1) * height);		vertex_buffer_data.push_back(v);

			index_buffer_data.push_back((row * numCol + col) * 4 + 0);
			index_buffer_data.push_back((row * numCol + col) * 4 + 1);
			index_buffer_data.push_back((row * numCol + col) * 4 + 2);
			index_buffer_data.push_back((row * numCol + col) * 4 + 0);
			index_buffer_data.push_back((row * numCol + col) * 4 + 2);
			index_buffer_data.push_back((row * numCol + col) * 4 + 3);
		}
	}

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateBB(const std::string &meshName, Vector3 max, Vector3 min)
{
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	Vertex v;

	v.pos.Set(min.x, min.y, min.z); vertex_buffer_data.push_back(v); // --+ 0
	v.pos.Set(min.x, max.y, min.z); vertex_buffer_data.push_back(v); // -++ 1 
	v.pos.Set(min.x, min.y, max.z); vertex_buffer_data.push_back(v); // --- 2
	v.pos.Set(min.x, max.y, max.z); vertex_buffer_data.push_back(v); // -+- 3

	v.pos.Set(max.x, min.y, min.z); vertex_buffer_data.push_back(v); // +-+ 4
	v.pos.Set(max.x, max.y, min.z); vertex_buffer_data.push_back(v); // +++ 5
	v.pos.Set(max.x, min.y, max.z); vertex_buffer_data.push_back(v); // +-- 6
	v.pos.Set(max.x, max.y, max.z); vertex_buffer_data.push_back(v); // ++- 7


	index_buffer_data.push_back(0);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(1);

	index_buffer_data.push_back(4);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(5);

	index_buffer_data.push_back(7);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(7);
	//045051 467475 762237 132201 264402 315357
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);

	index_buffer_data.push_back(2);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(2);

	index_buffer_data.push_back(3);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(7);

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_LINES;

	return mesh;
}