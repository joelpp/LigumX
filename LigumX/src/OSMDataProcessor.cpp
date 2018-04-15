#include "OSMDataProcessor.h"

#include "glm/glm.hpp"

#include "Mesh.h"
#include "Material.h"
#include "Model.h"
#include "Renderer.h"

#include "Entity.h"
#include "LigumX.h"
#include "World.h"
#include "Sector.h"
#include "SectorGraphicalData.h"

#include "Node.h"
#include "Way.h"


#pragma region  CLASS_SOURCE OSMDataProcessor
OSMDataProcessor* g_OSMDataProcessor;

#include "OSMDataProcessor.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData OSMDataProcessor::g_Properties[] = 
{
{ "ObjectID", PIDX_ObjectID, offsetof(OSMDataProcessor, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "Name", PIDX_Name, offsetof(OSMDataProcessor, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
{ "RoadWidth", PIDX_RoadWidth, offsetof(OSMDataProcessor, m_RoadWidth), 0, LXType_float, false, LXType_None, 0, 0, 0, }, 
};
bool OSMDataProcessor::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}

#pragma endregion  CLASS_SOURCE OSMDataProcessor


Mesh* OSMDataProcessor::BuildRoadMesh(Sector* sector, Way* way)
{
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> vertices;
	glm::vec3 up = glm::vec3(0, 0, 1);

	for (auto nodeIt = way->GetNodes().begin(); nodeIt != (way->GetNodes().end() - 1); ++nodeIt)
	{
		Node* node = *nodeIt;
		Node* nextNode = *(nodeIt + 1);
		glm::vec3 nodePos = node->GetWorldPosition();
		glm::vec3 nextPos = nextNode->GetWorldPosition();

		glm::vec3 segment = nextPos - nodePos;
		float distance = glm::length(segment);

		if (distance == 0.f)
		{
			continue;
		}

		glm::vec3 direction = glm::normalize(segment);
		glm::vec3 right = glm::cross(direction, up);

		float offset = 30.f;
		direction *= distance;
		glm::vec3 first = nodePos - offset * right;
		offset *= 2;

		glm::vec3 second = first + right * offset;
		if (vertices.size() > 0)
		{
			glm::vec3 old2 = vertices[vertices.size() - 2];
			glm::vec3 old = vertices[vertices.size() - 1];
			vertices.push_back(old);
			vertices.push_back(old2);
			vertices.push_back(first);
			vertices.push_back(old2);
			vertices.push_back(second);
			vertices.push_back(first);

			uvs.push_back(glm::vec2(0, 0));
			uvs.push_back(glm::vec2(1, 0));
			uvs.push_back(glm::vec2(0, 1));
			uvs.push_back(glm::vec2(1, 0));
			uvs.push_back(glm::vec2(1, 1));
			uvs.push_back(glm::vec2(0, 1));
		}

		vertices.push_back(first);
		vertices.push_back(second);
		vertices.push_back(first + direction);
		vertices.push_back(second);
		vertices.push_back(second + direction);
		vertices.push_back(first + direction);

		uvs.push_back(glm::vec2(0, 0));
		uvs.push_back(glm::vec2(1, 0));
		uvs.push_back(glm::vec2(0, 1));
		uvs.push_back(glm::vec2(1, 0));
		uvs.push_back(glm::vec2(1, 1));
		uvs.push_back(glm::vec2(0, 1));
	}

	if (vertices.size() > 0)
	{
		Mesh* roadMesh = new Mesh(vertices, uvs, GL::PrimitiveMode::Triangles, false);

		return roadMesh;
	}

	return nullptr;
}

void OSMDataProcessor::ProcessRoad(Sector* sector, Way* way)
{
	Mesh* roadMesh = BuildRoadMesh(sector, way);

	if (roadMesh != nullptr)
	{
		Renderer& renderer = Renderer::GetInstance();

		Model* roadModel = new Model();
		roadModel->addMesh(roadMesh, new Material(ShaderFamily_Roads));
		roadModel->SetName("Road_Test");

		Entity* roadEntity = new Entity();
		roadEntity->SetName("Road - " + way->GetName());
		roadEntity->SetModel(roadModel);
		
		roadEntity->SetVisible(true);

		//World* world = LigumX::GetInstance().GetWorld();
		//world->AddTo_Entities(roadEntity);

		sector->GetGraphicalData()->GetRoadEntities().push_back(roadEntity);
	}
}

void OSMDataProcessor::ProcessSector(Sector* sector)
{
	for (auto it = sector->m_Data->ways.begin(); it != sector->m_Data->ways.end(); ++it)
	{
		Way* way = it->second;

		bool hsNodes = (way->GetNodes().size() > 0);

		bool isRoad = way->GetOSMElementType() >= OSMElementType_HighwayTrunk && way->GetOSMElementType() <= OSMElementType_HighwayUnclassified;
		if (isRoad)
		{
			ProcessRoad(sector, way);
			continue;
		}

	}
}
