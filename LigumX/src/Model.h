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
#include "LXSystem.h"
class Serializer2;

class Model;
class Material;
class Mesh;


#pragma endregion  FORWARD_DECLARATIONS Model
class Mesh;
class Texture;
class ProgramPipeline;
class Material;


#pragma region  HEADER Model
class Model : public LXObject
{
public:
static const int ClassID = -1333542246;
static const LXType Type = LXType_Model;
static constexpr const char* ClassName = "Model";
virtual LXType GetLXType() { return LXType_Model; }
virtual const char* GetLXClassName() { return ClassName; }
typedef LXObject super;

const std::string& GetFilename() { return m_Filename; }; 
void SetFilename(std::string value) { m_Filename = value; }; 
std::vector<Material*>& GetMaterials() { return m_Materials; }; 
void SetMaterials(std::vector<Material*> value) { m_Materials = value; }; 
void AddTo_Materials(Material* value) { m_Materials.push_back(value); };
std::vector<Mesh*>& GetMeshes() { return m_Meshes; }; 
void SetMeshes(std::vector<Mesh*> value) { m_Meshes = value; }; 
void AddTo_Meshes(Mesh* value) { m_Meshes.push_back(value); };
private:
std::string m_Filename;
std::vector<Material*> m_Materials;
std::vector<Mesh*> m_Meshes;
public:
static const int g_PropertyCount = 3;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_ModelPIDX
{
PIDX_Filename,
PIDX_Materials,
PIDX_Meshes,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
void PostSerialization(bool writing, bool success);
virtual bool ShowPropertyGrid();
virtual void Clone(Model& other);
virtual const char* GetTypeName();

#pragma endregion  HEADER Model

public:

	Model();
	Model(std::string path);
	Model(Mesh* mesh, Material* material);

	Model(std::vector<Mesh* > meshList, std::vector<Material* > materialList);

	void LoadModel();
    void processNode(aiNode* node, const aiScene* scene);
	Material* processMaterial(aiMesh* assimpMesh, const aiScene* scene);
    Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, 
                                             std::string typeName);
	void addMesh(Mesh* mesh, Material* material);
	void addMesh(Mesh* mesh);

	bool GetMinMax(glm::vec3& min, glm::vec3& max);

	glm::vec3 position;
    glm::mat4 m_modelMatrix;
	glm::mat4x4 m_ModelToWorldMatrix;

	void CreateHWBuffers();

};
