#pragma once

#include <vector>
#include <string>
#include <sstream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glad/glad.h"
#include "Logging.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

class Mesh;
class Texture;
class ProgramPipeline;
class Material;

class Model{
public:

	Model();
	Model(std::string path);
	Model(std::string name, std::string path);
	Model(Mesh* mesh, Material* material);

	Model(std::vector<Mesh* > meshList, std::vector<Material* > materialList);

	void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
	Material* processMaterial(aiMesh* assimpMesh, const aiScene* scene);
    Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, 
                                             std::string typeName);
	void addMesh(Mesh* mesh, Material* material);


	std::vector<Mesh* > m_meshes;
	std::vector<Material* > m_materialList;

    std::string name;
	glm::vec3 position;
    glm::mat4 m_modelMatrix;
	glm::mat4x4 m_ModelToWorldMatrix;

	void CreateHWBuffers();


private:
	REGISTERCLASS(Model);
};