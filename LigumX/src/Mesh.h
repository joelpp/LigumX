#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "GL.h"
#include "Logging.h"
#include <iostream>

class Texture;
class Material;
class Vertex;
class Mesh;

class DefaultMeshes
{
public:
	DefaultMeshes();
	Mesh* DefaultQuadMesh;
	Mesh* DefaultSphereMesh;
	Mesh* DefaultCubeMesh;

	void InitializeDefaultCube();
	void InitializeDefaultSphere();
	void InitializeDefaultQuad();
};

extern DefaultMeshes* g_DefaultMeshes;

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