#include "DefaultObjects.h"
#include "Entity.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"
#include "ObjectManager.h"

DefaultObjects* g_DefaultObjects;

void DefaultObjects::InitializeDefaultCube()
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


void DefaultObjects::InitializeDefaultQuad()
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

void DefaultObjects::InitializeDefaultTerrainMesh()
{
	DefaultTerrainMesh = new Mesh();

	// todo try morgan's cube obj in here
	std::vector<glm::vec3>& points = DefaultTerrainMesh->m_buffers.vertexPositions;
	std::vector<glm::vec2>& UVs = DefaultTerrainMesh->m_buffers.m_vertexUVs;
	std::vector<glm::vec3>& normals = DefaultTerrainMesh->m_buffers.m_vertexNormals;
	std::vector<int>& indexBuffer = DefaultTerrainMesh->m_buffers.indexBuffer;

	int iWidth = 512;

	for (int i = 0; i < iWidth; i++)
	{
		for (int j = 0; j < iWidth; j++)
		{
			float last = iWidth - 1.f;
			float x = (float)i / (float)last;
			float y = (float)j / (float)last;
			points.push_back(glm::vec3(x, y, 0));
			UVs.push_back(glm::vec2(x, y));
		}
	}

	// vertically
	for (int i = 0; i < iWidth - 1; ++i)
	{
		//horizontally
		for (int j = 0; j < iWidth - 1; ++j)
		{
			// bottom left
			indexBuffer.push_back((i * iWidth) + j);
			// bottom right
			indexBuffer.push_back(((i + 1) * iWidth) + (j));
			// top left
			indexBuffer.push_back((i * iWidth) + (j + 1));

			// bottom right
			indexBuffer.push_back(((i + 1) * iWidth) + (j));
			// top left
			indexBuffer.push_back((i * iWidth) + (j + 1));
			// top right
			indexBuffer.push_back(((i + 1) * iWidth) + (j + 1));
		}
	}

	DefaultTerrainMesh->createBuffers();
	DefaultTerrainMesh->m_usesIndexBuffer = true;
	DefaultTerrainMesh->m_renderingMode = GL_TRIANGLES;

	DefaultTerrainMesh->SetName("Default Terrain Mesh");
	DefaultTerrainMesh->SetObjectID(g_ObjectManager->DefaultTerrainMeshID);
	g_ObjectManager->AddObject(LXType_Mesh, DefaultTerrainMesh);
}


void DefaultObjects::InitializeDefaultSphere()
{
	Model* testModel = new Model("sphere/sphere.obj");
	testModel->loadModel();
	DefaultSphereMesh = testModel->m_meshes[0];
	DefaultSphereMesh->SetName("Default Sphere Mesh");
	DefaultSphereMesh->SetObjectID(g_ObjectManager->DefaultSphereMeshID);

	g_ObjectManager->AddObject(LXType_Mesh, DefaultSphereMesh);
}

DefaultObjects::DefaultObjects()
{
	InitializeDefaultQuad();
	InitializeDefaultSphere();
	InitializeDefaultCube();
	InitializeDefaultTerrainMesh();

	InitializeManipulator();
}


Mesh* DefaultObjects::GetMeshFromID(int id)
{
	if (!g_ObjectManager->IsHardcodedID(id))
	{
		return nullptr;
	}

	return (Mesh*) (g_ObjectManager->FindObjectByID(id, LXType_Mesh, false));
}

void DefaultObjects::InitializeManipulator()
{
	DefaultManipulatorEntity = new Entity();
	DefaultManipulatorEntity->SetObjectID(g_ObjectManager->DefaultManipulatorEntityID);
	DefaultManipulatorEntity->SetName("Manipulator Entity");
	DefaultManipulatorEntity->SetPickingID(0.15f);
	DefaultManipulatorEntity->SetVisible(true);

	DefaultRedMaterial = new Material();
	DefaultRedMaterial->SetName("Default red material");
	DefaultRedMaterial->SetObjectID(g_ObjectManager->DefaultRedMaterialID);
	DefaultRedMaterial->SetDiffuseColor(glm::vec3(1, 0, 0));
	DefaultRedMaterial->SetUnlit(true);

	DefaultCubeModel = new Model();
	DefaultCubeModel->SetName("Default cube model");
	DefaultCubeModel->SetObjectID(g_ObjectManager->DefaultCubeModelID);
	DefaultCubeModel->addMesh(DefaultCubeMesh, DefaultRedMaterial);

	DefaultManipulatorEntity->SetModel(DefaultCubeModel);
}