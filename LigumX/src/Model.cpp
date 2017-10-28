#include "stdafx.h"
#include "stringutils.h"

#include "Model.h"	
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "program_pipeline.h"
#include "DefaultMeshes.h"
#include "glm/gtc/matrix_transform.hpp"

#pragma region  CLASS_SOURCE Model

#include "Model.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData Model::g_Properties[] = 
{
{ "ObjectID", offsetof(Model, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "Name", offsetof(Model, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
{ "Filename", offsetof(Model, m_Filename), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
{ "Materials", offsetof(Model, m_Materials), 0, LXType_stdvector, false, LXType_Material, 0, 0, 0, }, 
};
void Model::Serialize(bool writing)
{
	g_Serializer->SerializeObject(this, writing); 
	PostSerialization(writing);
}

#pragma endregion  CLASS_SOURCE Model


Model::Model()
{
	m_modelMatrix = glm::mat4(1.0);
	m_ObjectID = g_ObjectManager->GetNewObjectID();
}

Model::Model(std::string path)
	: m_Filename(path)
{
	m_modelMatrix = glm::mat4(1.0);
	m_ObjectID = g_ObjectManager->GetNewObjectID();
}

Model::Model(Mesh* mesh, Material* material)
{
	m_meshes.push_back(mesh);

	if (material != nullptr)
	{
		m_Materials.push_back(material);
	}
	m_ObjectID = g_ObjectManager->GetNewObjectID();
}

Model::Model(std::vector<Mesh* > meshList, std::vector<Material* > materialList)
{
	m_meshes = meshList;
	m_Materials = materialList;
	m_ObjectID = g_ObjectManager->GetNewObjectID();
}

void Model::PostSerialization(bool writing)
{
	if (!writing) // reading
	{
		if (g_ObjectManager->FilenameIsID(m_Filename))
		{
			StringList list;
			list.push_back("<");
			list.push_back(">");

			std::string filenameCopy = StringUtils::RemoveSubstrings(m_Filename, list);

			int hardcodedMeshID = std::atoi(filenameCopy.c_str());
			Mesh* mesh = g_DefaultMeshes->GetMeshFromID(hardcodedMeshID);
			m_meshes.push_back(mesh);
		}
		else
		{
			loadModel();
		}
	}
}


void Model::CreateHWBuffers()
{
	for (Mesh* mesh : m_meshes)
	{
		mesh->createBuffers();
		mesh->m_usesIndexBuffer = true;
		mesh->m_renderingMode = GL_TRIANGLES;
	}
}

void Model::loadModel()
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(m_Filename, aiProcess_Triangulate | /*aiProcess_GenNormals |*/ aiProcess_GenSmoothNormals);
	
    if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
    	std::stringstream ss;
    	ss << "ERROR::ASSIMP::" << import.GetErrorString();
        PRINT(ss.str());
        return;
    }
	
    this->processNode(scene->mRootNode, scene);

	CreateHWBuffers();
}  

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // Process all the node's meshes (if any)
    for(GLuint i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* assimpMesh = scene->mMeshes[node->mMeshes[i]]; 
		
		Mesh* mesh = processMesh(assimpMesh, scene);

		addMesh(mesh);
		//addMesh(g_DefaultMeshes->DefaultQuadMesh, material);
    }
    // Then do the same for each of its children
    for(GLuint i = 0; i < node->mNumChildren; i++)
    {
        this->processNode(node->mChildren[i], scene);
    }
}  

std::string concatenate(std::string string, aiString aistring)
{
	return string + aistring.C_Str();
}

glm::vec3 toVec3(aiColor3D color)
{
	return glm::vec3(color.r, color.g, color.b);
}

Texture* tryLoadTexture(std::string path)
{
	Texture* texture;
	try
	{
		texture = new Texture(path);
		texture->Initialize();
	}
	catch (std::exception e)
	{
		texture = nullptr;
	}

	return texture;
}

Material* Model::processMaterial(aiMesh* assimpMesh, const aiScene* scene)
{
	Material* material = new Material();

	aiMaterial* mat = scene->mMaterials[assimpMesh->mMaterialIndex];

	std::string texturePath = "C:/Users/Joel/Documents/LigumX/LigumX/data/models/nanosuit/tex/";

	if (mat)
	{
		aiString fileDiffuse;
		mat->GetTexture(aiTextureType_DIFFUSE, 0, &fileDiffuse);

		aiString fileSpecular;
		mat->GetTexture(aiTextureType_SPECULAR, 0, &fileSpecular);

		aiColor3D diffuseColor(0.f, 0.f, 0.f);
		mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);

		aiColor3D specularColor(0.f, 0.f, 0.f);
		mat->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);

		float shininess(0.f);
		mat->Get(AI_MATKEY_SHININESS, shininess);

		aiColor3D ambientColor(0.f, 0.f, 0.f);
		mat->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);

		Texture* diffuseTexture = tryLoadTexture(concatenate(texturePath, fileDiffuse));

		Texture* specularTexture = tryLoadTexture(concatenate(texturePath, fileSpecular));

		material->SetDiffuseTexture(diffuseTexture);
		material->SetSpecularTexture(specularTexture);
		material->SetAmbientColor(toVec3(ambientColor));
		material->SetDiffuseColor(toVec3(diffuseColor));
		material->SetSpecularColor(toVec3(specularColor));
		material->SetShininess(shininess);
	}

	return material;
}

Mesh* Model::processMesh(aiMesh* assimpMesh, const aiScene* scene)
{
	Mesh* newMesh = new Mesh();
    // std::vector<GLuint> indices;
    // std::vector<Texture> textures;

    for(GLuint i = 0; i < assimpMesh->mNumVertices; i++)
    {
        // Process vertex positions, normals and texture coordinates
        glm::vec3 position; 
		position.x = assimpMesh->mVertices[i].x;
		position.y = assimpMesh->mVertices[i].y;
		position.z = assimpMesh->mVertices[i].z; 
		newMesh->m_buffers.vertexPositions.push_back(position);

		glm::vec2 texCoords;
		texCoords.x = assimpMesh->mTextureCoords[0][i].x;
		texCoords.y = assimpMesh->mTextureCoords[0][i].y;

		newMesh->m_buffers.m_vertexUVs.push_back(texCoords);


		glm::vec3 normal;
		normal.x = assimpMesh->mNormals[i].x;
		normal.y = assimpMesh->mNormals[i].y;
		normal.z = assimpMesh->mNormals[i].z;

		newMesh->m_buffers.m_vertexNormals.push_back(normal);
    }

	for (GLuint i = 0; i < assimpMesh->mNumFaces; i++)
	{
		aiFace face = assimpMesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			newMesh->m_buffers.indexBuffer.push_back(face.mIndices[j]);
	}

    return newMesh;
}  

void Model::addMesh(Mesh* mesh)
{
	m_meshes.push_back(mesh);
}

void Model::addMesh(Mesh* mesh, Material* material)
{
	m_Materials.push_back( material ) ;
	m_meshes.push_back(mesh);

}

