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

#pragma region  FORWARD_DECLARATIONS Model
#include "property.h"

#pragma endregion  FORWARD_DECLARATIONS Model
class Mesh;
class Texture;
class ProgramPipeline;
class Material;

class Model
{
#pragma region  HEADER Model
static const int ClassID = 2189814010;
public:
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; };
private:
std::string m_Name;
public:
static const int g_ModelPropertyCount = 1;
static const ClassPropertyData g_Properties[g_ModelPropertyCount];


#pragma endregion  HEADER Model

public:

	Model();
	Model(std::string path);
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

	glm::vec3 position;
    glm::mat4 m_modelMatrix;
	glm::mat4x4 m_ModelToWorldMatrix;

	void CreateHWBuffers();


private:
	REGISTERCLASS(Model);
};
