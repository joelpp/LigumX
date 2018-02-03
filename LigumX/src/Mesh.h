#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "GL.h"
#include "Logging.h"
#include <iostream>

#pragma region  FORWARD_DECLARATIONS Mesh
#include "property.h"

class Mesh;


#pragma endregion  FORWARD_DECLARATIONS Mesh
class Texture;
class Material;
class Vertex;

enum EBufferType
{
	VERTEX_POSITIONS,
	VERTEX_NORMALS,
	VERTEX_UVS
};

class GPUBuffers
{
public:

	GLuint glidPositions;
	GLuint glidNormals;
	GLuint glidUVs;
	GLuint glidIndexBuffer;
};

class FlatWaysGPUBuffers : public GPUBuffers
{
public:
	GLuint glidWayTypeBuffer;
};

class CPUBuffers
{
public:

	std::vector<glm::vec3> vertexPositions;
	std::vector<glm::vec3> m_vertexNormals;
	std::vector<glm::vec2> m_vertexUVs;
	std::vector<int> indexBuffer;
};

class FlatWaysCPUBuffers : public CPUBuffers
{
public:
	std::vector<int> WayTypeBuffer;
};


class Mesh
{
#pragma region  HEADER Mesh
public:
static const int ClassID = 132142556;
static const LXType Type = LXType_Mesh;
static constexpr const char* ClassName = "Mesh";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
const bool& GetUsesIndexBuffer() { return m_UsesIndexBuffer; }; 
void SetUsesIndexBuffer(bool value) { m_UsesIndexBuffer = value; }; 
const bool& GetWireframeRendering() { return m_WireframeRendering; }; 
void SetWireframeRendering(bool value) { m_WireframeRendering = value; }; 
const bool& GetPointRendering() { return m_PointRendering; }; 
void SetPointRendering(bool value) { m_PointRendering = value; }; 
private:
int m_ObjectID;
std::string m_Name;
bool m_UsesIndexBuffer = false;
bool m_WireframeRendering = false;
bool m_PointRendering = false;
public:
static const int g_PropertyCount = 5;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_MeshPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_UsesIndexBuffer,
PIDX_WireframeRendering,
PIDX_PointRendering,
};
bool Serialize(bool writing);

#pragma endregion  HEADER Mesh


public:
	// Mesh(std::vector<Vertex> vertexArray);
	// void addVertex(Vertex v);
	Mesh();
	Mesh(const std::vector<glm::vec3>& vertices, GLenum renderingMode);
	Mesh(const std::vector<glm::vec3>& vertices, GLenum renderingMode, bool usePointRendering);
	Mesh(const std::vector<glm::vec3>& vertices, const std::vector<int>& indices, GLenum renderingMode, bool usePointRendering);
	
	void padBuffer(EBufferType bufferType);
	virtual void createBuffers();
	std::vector<int> m_indexArray;

	CPUBuffers m_buffers;


	GLuint m_VAO;

	GLenum m_renderingMode;
	
	virtual GPUBuffers& GetGPUBuffers() { return m_VBOs; }
private:
	GPUBuffers m_VBOs;

};
