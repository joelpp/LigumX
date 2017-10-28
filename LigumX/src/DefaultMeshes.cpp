#include "DefaultMeshes.h"
#include "Mesh.h"
#include "Model.h"
#include "ObjectManager.h"

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

	Model* testModel = new Model("cube/cube.obj");
	testModel->loadModel();
	DefaultCubeMesh = testModel->m_meshes[0];
	DefaultCubeMesh->SetName("Default Cube Mesh");
	DefaultCubeMesh->SetObjectID(g_ObjectManager->DefaultCubeMeshID);

	g_ObjectManager->AddObject(LXType_Mesh, DefaultCubeMesh);

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

	DefaultQuadMesh->SetName("Default Quad Mesh");
	DefaultQuadMesh->SetObjectID(g_ObjectManager->DefaultQuadMeshID);
	g_ObjectManager->AddObject(LXType_Mesh, DefaultQuadMesh);
}

void DefaultMeshes::InitializeDefaultSphere()
{
	Model* testModel = new Model("sphere/sphere.obj");
	testModel->loadModel();
	DefaultSphereMesh = testModel->m_meshes[0];
	DefaultSphereMesh->SetName("Default Sphere Mesh");
	DefaultSphereMesh->SetObjectID(g_ObjectManager->DefaultSphereMeshID);

	g_ObjectManager->AddObject(LXType_Mesh, DefaultSphereMesh);
}

DefaultMeshes::DefaultMeshes()
{
	InitializeDefaultQuad();
	InitializeDefaultSphere();
	InitializeDefaultCube();
}


Mesh* DefaultMeshes::GetMeshFromID(int id)
{
	if (!g_ObjectManager->IsHardcodedID(id))
	{
		return nullptr;
	}

	if (id == g_ObjectManager->DefaultCubeMeshID)
	{
		return DefaultCubeMesh;
	}
	else if (id == g_ObjectManager->DefaultQuadMeshID)
	{
		return DefaultQuadMesh;
	}
	else if (id == g_ObjectManager->DefaultSphereMeshID)
	{
		return DefaultSphereMesh;
	}

	return nullptr;
}