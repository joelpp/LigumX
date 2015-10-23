#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "GL/glew.h"
#include "Logging.h"
#include <iostream>
class Texture;
class Material;
class Vertex;

struct GPUBuffers{
	GLuint glidPositions;
	GLuint glidNormals;
	GLuint glidUVs;
};

struct CPUBuffers
{
	std::vector<glm::vec3> m_vertexPositions;
	std::vector<glm::vec3> m_vertexNormals;
	std::vector<glm::vec2> m_vertexUVs;
};

class Mesh{
public:
	// Mesh(std::vector<Vertex> vertexArray);
	// void addVertex(Vertex v);
	Mesh();

	void createBuffers();
	std::vector<int> m_indexArray;

	GPUBuffers m_VBOs;
	CPUBuffers m_buffers;


	GLuint m_VAO;

private:
	REGISTERCLASS(Mesh);
};