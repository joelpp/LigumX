#pragma once

#include <vector>
#include <string>
#include <sstream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Logging.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#pragma region  FORWARD_DECLARATIONS Model
#include "property.h"
class Material;

#pragma endregion  FORWARD_DECLARATIONS Model
class Mesh;
class Texture;
class ProgramPipeline;
class Material;

class Model
{
public:
public:
public:
#pragma region  HEADER Model
public:
static const int ClassID = 2189814010;
static constexpr const char* ClassName = "Model";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
const std::string& GetFilename() { return m_Filename; }; 
void SetFilename(std::string value) { m_Filename = value; }; 
std::vector<Material*>& GetMaterials() { return m_Materials; }; 
void SetMaterials(std::vector<Material*> value) { m_Materials = value; }; 
const bool& GetFilenameIsID() { return m_FilenameIsID; }; 
void SetFilenameIsID(bool value) { SetFilenameIsIDCallback(value); }; 
void SetFilenameIsIDCallback(bool value);
private:
int m_ObjectID;
std::string m_Name;
std::string m_Filename;
std::vector<Material*> m_Materials;
bool m_FilenameIsID;
public:
static const int g_PropertyCount = 5;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_ModelPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_Filename,
PIDX_Materials,
PIDX_FilenameIsID,
};
void Serialize(bool writing);
void PostSerialization(bool writing);

#pragma endregion  HEADER Model

public:

	Model();
	Model(std::string path);
	Model(Mesh* mesh, Material* material);

	Model(std::vector<Mesh* > meshList, std::vector<Material* > materialList);

	void loadModel();
    void processNode(aiNode* node, const aiScene* scene);
	Material* processMaterial(aiMesh* assimpMesh, const aiScene* scene);
    Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, 
                                             std::string typeName);
	void addMesh(Mesh* mesh, Material* material);
	void addMesh(Mesh* mesh);


	std::vector<Mesh* > m_meshes;

	glm::vec3 position;
    glm::mat4 m_modelMatrix;
	glm::mat4x4 m_ModelToWorldMatrix;

	void CreateHWBuffers();


private:
	REGISTERCLASS(Model);
};
