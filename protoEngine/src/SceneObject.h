#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "GL/glew.h"

struct Material{
	GLenum mode;
	glm::vec3 albedo;
	std::vector<Texture*>;
};

class Mesh;

class SceneObject{

	glm::vec3 position;

	std::vector<Mesh> m_meshes;

	std::vector<Material> materialList;
};