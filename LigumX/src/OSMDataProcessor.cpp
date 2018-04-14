#include "OSMDataProcessor.h"

#include "glm/glm.hpp"

#include "Mesh.h"
#include "Material.h"
#include "Model.h"
#include "Renderer.h"

#include "Entity.h"
#include "Sector.h"

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

void OSMDataProcessor::BuildRoadMeshes(Sector* sector, Way* way)
{
	std::vector<glm::vec3> vertices;
	glm::vec3 up = glm::vec3(0, 0, 1);

	for (auto nodeIt = way->GetNodes().begin(); nodeIt != (way->GetNodes().end() - 1); ++nodeIt)
	{
		float offset = 3.f;
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
#if 0 

		glm::vec3 right = glm::cross(direction, up);

		glm::vec3 first = nodePos - offset * right;

		vertices.push_back(first);
		vertices.push_back(first + glm::vec3(offset, 0, 0) * right);
		vertices.push_back(first + glm::vec3(0, offset, 0) * direction);
		vertices.push_back(first + glm::vec3(offset, 0, 0) * right);
		vertices.push_back(first + glm::vec3(0, offset, 0) * direction);
		vertices.push_back(first + glm::vec3(offset, offset, 0) * (direction + right));
#else
		right *= 10.f;
		direction *= distance;
		glm::vec3 first = nodePos - offset * right;
		offset *= 2;

		glm::vec3 second = first + right * offset;
		if (vertices.size() > 0)
		{
			glm::vec3 old2 = vertices[vertices.size() - 2];
			glm::vec3 old = vertices[vertices.size() - 1];
			vertices.push_back(old2);
			vertices.push_back(old);
			vertices.push_back(first);
			vertices.push_back(old);
			vertices.push_back(first);
			vertices.push_back(second);
		}

		vertices.push_back(first);
		vertices.push_back(second);
		vertices.push_back(first + direction);
		vertices.push_back(second);
		vertices.push_back(first + direction);
		vertices.push_back(first + direction + right * offset);
#endif

		if (nodeIt != way->GetNodes().begin())
		{

		}
	}

	if (vertices.size() > 0)
	{
		Renderer& renderer = Renderer::GetInstance();

		Mesh* roadMesh = new Mesh(vertices, GL::PrimitiveMode::Triangles, false);
		Model* roadModel = new Model();
		roadModel->addMesh(roadMesh, new Material(renderer.pPipelineBasic, glm::vec3(1, 1, 1)));
		roadModel->SetName("Road_Test");
		renderer.AddToDebugModels(roadModel);
	}
}

void OSMDataProcessor::ProcessRoad(Sector* sector, Way* way)
{
	BuildRoadMeshes(sector, way);
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
