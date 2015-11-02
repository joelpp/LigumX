#include "Model.h"	
#include "Mesh.h"
#include "Material.h"
#include "glm/gtc/matrix_transform.hpp"

Model::Model()
{

}

Model::Model(std::string path)
{
	loadModel(path);
	m_modelMatrix = glm::mat4(1.0);
}

void Model::loadModel(std::string path)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate );	
	
    if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
    	std::stringstream ss;
    	ss << "ERROR::ASSIMP::" << import.GetErrorString();
        PRINT(ss.str());
        return;
    }

    this->processNode(scene->mRootNode, scene);
}  

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // Process all the node's meshes (if any)
    for(GLuint i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; 
        this->m_meshes.push_back(this->processMesh(mesh, scene));			
    }
    // Then do the same for each of its children
    for(GLuint i = 0; i < node->mNumChildren; i++)
    {
        this->processNode(node->mChildren[i], scene);
    }
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
    }

    // // Process indices
    // ...
    // // Process material
    // if(mesh->mMaterialIndex >= 0)
    // {
    //     ...
    // }

    return newMesh;
}  

void Model::addMesh(Mesh* mesh, Material* material)
{

	m_materialList.push_back( material ) ;

	m_meshes.push_back(mesh);
}



