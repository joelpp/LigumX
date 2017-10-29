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

struct GPUBuffers{
	GLuint glidPositions;
	GLuint glidNormals;
	GLuint glidUVs;
	GLuint glidIndexBuffer;
};

struct CPUBuffers
{
	std::vector<glm::vec3> vertexPositions;
	std::vector<glm::vec3> m_vertexNormals;
	std::vector<glm::vec2> m_vertexUVs;
	std::vector<int> indexBuffer;
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
private:
int m_ObjectID;
std::string m_Name;
public:
static const int g_PropertyCount = 2;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_MeshPIDX
{
PIDX_ObjectID,
PIDX_Name,
};
void Serialize(bool writing);

#pragma endregion  HEADER Mesh


public:
	// Mesh(std::vector<Vertex> vertexArray);
	// void addVertex(Vertex v);
	Mesh();
	Mesh(std::vector<glm::vec3> vertices, GLenum renderingMode);
	Mesh(std::vector<glm::vec3> vertices, GLenum renderingMode, bool usePointRendering);
	
	void padBuffer(EBufferType bufferType);
	void createBuffers();
	std::vector<int> m_indexArray;

	GPUBuffers m_VBOs;
	CPUBuffers m_buffers;


	GLuint m_VAO;

	bool m_usesIndexBuffer;
	bool m_wireframeRendering;
	bool m_pointRendering;
	GLenum m_renderingMode;
	
private:
	REGISTERCLASS(Mesh);
};
