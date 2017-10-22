#include "DefaultMeshes.h"
#include "Mesh.h"
#include "Model.h"
#include "ObjectIDManager.h"

DefaultMeshes* g_DefaultMeshes;

void DefaultMeshes::InitializeDefaultCube()
{
	//	DefaultCubeMesh = new Mesh();
	//
	//	std::vector<glm::vec3>& points = DefaultQuadMesh->m_buffers.vertexPositions;
	//	std::vector<glm::vec2>& UVs = DefaultQuadMesh->m_buffers.m_vertexUVs;
	//	std::vector<glm::vec3>& normals = DefaultQuadMesh->m_buffers.m_vertexNormals;
	//	std::vector<int>& indexBuffer = DefaultQuadMesh->m_buffers.indexBuffer;
	//
	//	/*
	// points.push_back(glm::vec3(-0.5, 0.5, -0.5);
	// points.push_back(glm::vec3(-0.5, 0.5, 0.5);
	// points.push_back(glm::vec3(0.5, 0.5, 0.5);
	// points.push_back(glm::vec3(0.5, 0.5, -0.5);
	// points.push_back(glm::vec3(-0.5, -0.5, -0.5);
	// points.push_back(glm::vec3(-0.5 ,-0.5, 0.5);
	// points.push_back(glm::vec3(0.5, -0.5, 0.5);
	// points.push_back(glm::vec3(0.5, -0.5, -0.5);
	//
	//vt 0 1
	//vt 0 0
	//vt 1 0
	//vt 1 1
	//
	//vn 0 1 0
	//vn -1 0 0
	//vn 1 0 0
	//vn 0 0 -1
	//vn 0 0 1
	//vn 0 -1 0
	//*/
	//
	//	// bottom
	//	points.push_back(glm::vec3(-0.5, 0.5, -0.5));
	//	points.push_back(glm::vec3(-0.5, 0.5, 0.5));
	//	points.push_back(glm::vec3(0.5, 0.5, 0.5));
	//	points.push_back(glm::vec3(0.5, 0.5, -0.5));
	//	points.push_back(glm::vec3(-0.5, -0.5, -0.5));
	//	points.push_back(glm::vec3(-0.5, -0.5, 0.5));
	//	points.push_back(glm::vec3(0.5, -0.5, 0.5));
	//	points.push_back(glm::vec3(0.5, -0.5, -0.5));
	//
	//
	//	UVs.push_back(glm::vec2(0, 1));
	//	UVs.push_back(glm::vec2(0, 0));
	//	UVs.push_back(glm::vec2(1, 0));
	//	UVs.push_back(glm::vec2(1, 1));
	//
	//	vn 0 1 0
	//		vn - 1 0 0
	//		vn 1 0 0
	//		vn 0 0 - 1
	//		vn 0 0 1
	//		vn 0 - 1 0
	//
	//	normals.push_back(glm::vec3(0, 1, 0));
	//	normals.push_back(glm::vec3(-1, 0, 0));
	//	normals.push_back(glm::vec3(1, 0, 0));
	//	normals.push_back(glm::vec3(0, 0, -1));
	//	normals.push_back(glm::vec3(0, 0, 1));
	//	normals.push_back(glm::vec3(0,-1, 0));
	//
	//
	//
	//	indexBuffer.push_back(0);
	//	indexBuffer.push_back(1);
	//	indexBuffer.push_back(2);
	//	indexBuffer.push_back(2);
	//	indexBuffer.push_back(1);
	//	indexBuffer.push_back(3);
	//
	//	DefaultQuadMesh->createBuffers();
	//	DefaultQuadMesh->m_usesIndexBuffer = true;
	//	DefaultQuadMesh->m_renderingMode = GL_TRIANGLES;

	Model* testModel = new Model("C:/Users/Joel/Documents/LigumX/LigumX/data/models/cube/cube.obj");
	testModel->loadModel();
	DefaultCubeMesh = testModel->m_meshes[0];
	DefaultCubeMesh->SetObjectID(g_ObjectIDManager->DefaultCubeMeshID);

}


void DefaultMeshes::InitializeDefaultQuad()
{
	DefaultQuadMesh = new Mesh();

	// todo try morgan's cube obj in here
	std::vector<glm::vec3>& points = DefaultQuadMesh->m_buffers.vertexPositions;
	std::vector<glm::vec2>& UVs = DefaultQuadMesh->m_buffers.m_vertexUVs;
	std::vector<glm::vec3>& normals = DefaultQuadMesh->m_buffers.m_vertexNormals;
	std::vector<int>& indexBuffer = DefaultQuadMesh->m_buffers.indexBuffer;

	points.push_back(glm::vec3(0, 0, 0));
	points.push_back(glm::vec3(0, 1, 0));
	points.push_back(glm::vec3(1, 0, 0));
	points.push_back(glm::vec3(1, 1, 0));

	UVs.push_back(glm::vec2(0, 0));
	UVs.push_back(glm::vec2(0, 1));
	UVs.push_back(glm::vec2(1, 0));
	UVs.push_back(glm::vec2(1, 1));

	normals.push_back(glm::vec3(0, 0, 1));
	normals.push_back(glm::vec3(0, 0, 1));
	normals.push_back(glm::vec3(0, 0, 1));
	normals.push_back(glm::vec3(0, 0, 1));

	indexBuffer.push_back(0);
	indexBuffer.push_back(1);
	indexBuffer.push_back(2);
	indexBuffer.push_back(2);
	indexBuffer.push_back(1);
	indexBuffer.push_back(3);

	DefaultQuadMesh->createBuffers();
	DefaultQuadMesh->m_usesIndexBuffer = true;
	DefaultQuadMesh->m_renderingMode = GL_TRIANGLES;

	DefaultQuadMesh->SetObjectID(g_ObjectIDManager->DefaultQuadMeshID);
}

void DefaultMeshes::InitializeDefaultSphere()
{
	Model* testModel = new Model("C:/Users/Joel/Documents/LigumX/LigumX/data/models/sphere/sphere.obj");
	testModel->loadModel();
	DefaultSphereMesh = testModel->m_meshes[0];
	DefaultSphereMesh->SetObjectID(g_ObjectIDManager->DefaultSphereMeshID);

}

DefaultMeshes::DefaultMeshes()
{
	InitializeDefaultQuad();
	InitializeDefaultSphere();
	InitializeDefaultCube();

}


Mesh* DefaultMeshes::GetMeshFromID(int id)
{
	if (!g_ObjectIDManager->IsHardcodedID(id))
	{
		return nullptr;
	}

	if (id == g_ObjectIDManager->DefaultCubeMeshID)
	{
		return DefaultCubeMesh;
	}
	else if (id == g_ObjectIDManager->DefaultQuadMeshID)
	{
		return DefaultQuadMesh;
	}
	else if (id == g_ObjectIDManager->DefaultSphereMeshID)
	{
		return DefaultSphereMesh;
	}

	return nullptr;
}