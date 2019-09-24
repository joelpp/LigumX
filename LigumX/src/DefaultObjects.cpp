#include "DefaultObjects.h"

#include "Entity.h"
#include "Material.h"

#include "Mesh.h"
#include "Model.h"

#include "ObjectManager.h"
#include "EngineSettings.h"

DefaultObjects* g_DefaultObjects;

void DefaultObjects::InitializeDefaultCube()
{
	

	Model* testModel = new Model("cube/cube.obj");
	testModel->loadModel();
	DefaultCubeMesh = testModel->GetMeshes()[0];
	DefaultCubeMesh->SetName("Default Cube Mesh");
	DefaultCubeMesh->SetObjectID(g_ObjectManager->DefaultCubeMeshID);

	g_ObjectManager->AddObject(LXType_Mesh, DefaultCubeMesh);

}


void DefaultObjects::InitializeDefaultQuad()
{
	DefaultQuadMesh = new Mesh();

	// todo try morgan's cube obj in here
	std::vector<glm::vec3>& points = DefaultQuadMesh->m_buffers.GetVertexPositions();
	std::vector<glm::vec2>& UVs = DefaultQuadMesh->m_buffers.GetVertexUVs();
	std::vector<glm::vec3>& normals = DefaultQuadMesh->m_buffers.GetVertexNormals();
	std::vector<int>& indexBuffer = DefaultQuadMesh->m_buffers.GetIndexBuffer();

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

	DefaultQuadMesh->CreateBuffers();
	DefaultQuadMesh->SetUsesIndexBuffer(true);
	DefaultQuadMesh->m_PrimitiveMode = GL::PrimitiveMode::Triangles;

	DefaultQuadMesh->SetName("Default Quad Mesh");
	DefaultQuadMesh->SetObjectID(g_ObjectManager->DefaultQuadMeshID);
	g_ObjectManager->AddObject(LXType_Mesh, DefaultQuadMesh);
}

void DefaultObjects::InitializeDefaultTerrainMesh()
{
	DefaultTerrainMesh = new Mesh();

	// todo try morgan's cube obj in here
	std::vector<glm::vec3>& points = DefaultTerrainMesh->m_buffers.GetVertexPositions();
	std::vector<glm::vec2>& UVs = DefaultTerrainMesh->m_buffers.GetVertexUVs();
	std::vector<glm::vec3>& normals = DefaultTerrainMesh->m_buffers.GetVertexNormals();
	std::vector<int>& indexBuffer = DefaultTerrainMesh->m_buffers.GetIndexBuffer();

	int iWidth = 64;
	float uvScale = 1.f;//g_EngineSettings->GetTerrainTiling();

	for (int i = 0; i < iWidth; i++)
	{
		for (int j = 0; j < iWidth; j++)
		{
			float last = iWidth - 1.f;
			float x = (float)i / (float)last;
			float y = (float)j / (float)last;
			points.push_back(glm::vec3(x, y, 0));

			glm::vec2 uv = glm::vec2(x, y) * uvScale;
			UVs.push_back(uv);
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

	DefaultTerrainMesh->CreateBuffers();
	DefaultTerrainMesh->SetUsesIndexBuffer(true);
	DefaultTerrainMesh->m_PrimitiveMode = GL::PrimitiveMode::Triangles;

	DefaultTerrainMesh->SetName("Default Terrain Mesh");
	DefaultTerrainMesh->SetObjectID(g_ObjectManager->DefaultTerrainMeshID);
	g_ObjectManager->AddObject(LXType_Mesh, DefaultTerrainMesh);
}


void DefaultObjects::InitializeDefaultSphere()
{
	Model* testModel = new Model("sphere/sphere.obj");
	testModel->loadModel();
	DefaultSphereMesh = testModel->GetMeshes()[0];
	DefaultSphereMesh->SetName("Default Sphere Mesh");
	DefaultSphereMesh->SetObjectID(g_ObjectManager->DefaultSphereMeshID);

	g_ObjectManager->AddObject(LXType_Mesh, DefaultSphereMesh);
}

DefaultObjects::DefaultObjects()
{

}

void DefaultObjects::Initialize()
{
	InitializeDefaultCube();
	InitializeDefaultQuad();
	InitializeDefaultSphere();
	InitializeDefaultTerrainMesh();
	InitializeDefaultLine();

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

void DefaultObjects::InitializeDefaultLine()
{
	DefaultLineMesh = new Mesh();

	// todo try morgan's cube obj in here
	std::vector<glm::vec3>& points = DefaultLineMesh->m_buffers.GetVertexPositions();
	std::vector<glm::vec2>& UVs =	 DefaultLineMesh->m_buffers.GetVertexUVs();

	points.push_back(glm::vec3(0, 0, 0));
	points.push_back(glm::vec3(0, 1, 0));

	UVs.push_back(glm::vec2(0, 0));
	UVs.push_back(glm::vec2(1, 1));

	DefaultLineMesh->CreateBuffers();
	DefaultLineMesh->SetUsesIndexBuffer(true);
	DefaultLineMesh->m_PrimitiveMode = GL::PrimitiveMode::Lines;

	DefaultLineMesh->SetName("Default Line Mesh");
	DefaultLineMesh->SetObjectID(g_ObjectManager->DefaultLineMeshID);
}