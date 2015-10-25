#pragma once

#include <vector>
#include <string>
#include <sstream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "GL/glew.h"
#include "Logging.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Mesh;
class Texture;
class ProgramPipeline;
struct Material{
	ProgramPipeline* shader;
	glm::vec3 albedo;
	// std::vector<Texture*> textures;
};

class Model{
public:
	Model();
	Model(std::string path);
	void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, 
                                             std::string typeName);

	glm::vec3 position;

	std::vector<Mesh* > m_meshes;

	std::vector<Material> m_materialList;
    std::string directory;
    glm::mat4 m_modelMatrix;

private:
	REGISTERCLASS(Model);
};