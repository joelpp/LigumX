#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "Logging.h"
#include "LXError.h"

class Model;
class Way;


struct Triangle
{
	Triangle(const std::vector<glm::vec3>& vertices)
	{
		lxAssert(vertices.size() == 3);
		m_Vertices[0] = vertices[0];
		m_Vertices[1] = vertices[1];
		m_Vertices[2] = vertices[2];
	}

	glm::vec3 m_Vertices[3];
};

class Building
{
public:

	Model* m_Model;
	Way* m_Way;

	Building(Way* way);

	bool GenerateModel();

	const std::vector<Triangle>& GetTriangles() { return m_Triangles; }

	glm::vec3 m_MinCoords;
	glm::vec3 m_MaxCoords;
private:
	std::vector<Triangle> m_Triangles;



	REGISTERCLASS(Building);
};

