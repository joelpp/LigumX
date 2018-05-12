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

#include "Building.h"

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


float sign(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool PointInTriangle(const glm::vec3& pt, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3)
{
	bool b1, b2, b3;

	b1 = sign(pt, v1, v2) < 0.0f;
	b2 = sign(pt, v2, v3) < 0.0f;
	b3 = sign(pt, v3, v1) < 0.0f;

	return ((b1 == b2) && (b2 == b3));
}

Mesh* OSMDataProcessor::BuildAdressInterpolationBuilding(Sector* sector, Way* way)
{
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> vertices;
	glm::vec3 up = glm::vec3(0, 0, 1);

	const float buildingHeight = 100.f;

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

		glm::vec3 first = nodePos;
		glm::vec3 second = nodePos + up * buildingHeight;

		vertices.push_back(first);
		vertices.push_back(second);
		vertices.push_back(first + direction * distance);
		vertices.push_back(second);
		vertices.push_back(second + direction * distance);
		vertices.push_back(first + direction * distance);

		uvs.push_back(glm::vec2(0, 0));
		uvs.push_back(glm::vec2(1, 0));
		uvs.push_back(glm::vec2(0, 1));
		uvs.push_back(glm::vec2(1, 0));
		uvs.push_back(glm::vec2(1, 1));
		uvs.push_back(glm::vec2(0, 1));

	}

	if (vertices.size() > 0)
	{
		Mesh* buildingMesh = new Mesh(vertices, uvs, GL::PrimitiveMode::Triangles, false);

		return buildingMesh;
	}

	return nullptr;

}


Mesh* OSMDataProcessor::BuildGenericBuilding(Sector* sector, Way* way)
{
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> vertices;
	glm::vec3 up = glm::vec3(0, 0, 1);

	const float buildingHeight = 100.f;

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

		glm::vec3 first = nodePos;
		glm::vec3 second = nodePos + up * buildingHeight;

		vertices.push_back(first);
		vertices.push_back(second);
		vertices.push_back(first + direction * distance);
		vertices.push_back(second);
		vertices.push_back(second + direction * distance);
		vertices.push_back(first + direction * distance);

		uvs.push_back(glm::vec2(0, 0));
		uvs.push_back(glm::vec2(1, 0));
		uvs.push_back(glm::vec2(0, 1));
		uvs.push_back(glm::vec2(1, 0));
		uvs.push_back(glm::vec2(1, 1));
		uvs.push_back(glm::vec2(0, 1));

	}

	if (vertices.size() > 0)
	{
		Mesh* buildingMesh = new Mesh(vertices, uvs, GL::PrimitiveMode::Triangles, false);

		return buildingMesh;
	}

	return nullptr;

}



void OSMDataProcessor::ProcessGenericBuilding(Sector* sector, Way* way)
{
	Mesh* buildingMesh = BuildGenericBuilding(sector, way);

	if (buildingMesh != nullptr)
	{
		Renderer& renderer = Renderer::GetInstance();

		Model* buildingModel = new Model();
		Material* buildingMaterial = new Material(ShaderFamily_Basic);
		buildingMaterial->SetDiffuseTexture((Texture*)(g_ObjectManager->FindObjectByID(11039, LXType_Texture, true)));
		buildingMaterial->SetDiffuseTextureEnabled(true);

		buildingModel->addMesh(buildingMesh, buildingMaterial);
		buildingModel->SetName("Building_Test");


		Entity* buildingEntity = new Entity();
		buildingEntity->SetName("Building - " + way->GetName());
		buildingEntity->SetModel(buildingModel);

		buildingEntity->SetVisible(true);


		//World* world = LigumX::GetInstance().GetWorld();
		//world->AddTo_Entities(roadEntity);

		sector->GetGraphicalData()->GetStaticEntities().push_back(buildingEntity);
	}
}


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

void OSMDataProcessor::ProcessAddressInterpolation(Sector* sector, Way* way)
{
	Mesh* buildingMesh = nullptr;

	buildingMesh = BuildAdressInterpolationBuilding(sector, way);

	int nbBuildings = 5;
	float buildingWidth = 5.f;
	float buildingWOffset = 5.f;;


	if (buildingMesh != nullptr)
	{
		Renderer& renderer = Renderer::GetInstance();

		Model* buildingModel = new Model();
		Material* buildingMaterial = new Material(ShaderFamily_Basic);
		buildingMaterial->SetDiffuseTexture((Texture*)(g_ObjectManager->FindObjectByID(11039, LXType_Texture, true)));
		buildingMaterial->SetDiffuseTextureEnabled(true);

		buildingModel->addMesh(buildingMesh, buildingMaterial);
		buildingModel->SetName("AddrInterpolation_Test");


		Entity* buildingEntity = new Entity();
		buildingEntity->SetName("Building - " + way->GetName());
		buildingEntity->SetModel(buildingModel);

		buildingEntity->SetVisible(true);


		//World* world = LigumX::GetInstance().GetWorld();
		//world->AddTo_Entities(roadEntity);

		sector->GetGraphicalData()->GetStaticEntities().push_back(buildingEntity);
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

		bool isGenericBuilding = way->GetOSMElementType() >= OSMElementType_Building_Unmarked && way->GetOSMElementType() <= OSMElementType_Building_School;
		if (isGenericBuilding)
		{
			ProcessGenericBuilding(sector, way);
			continue;
		}

		bool isAddressInterpolation = way->GetOSMElementType() == OSMElementType_AddressInterpolation;
		if (isAddressInterpolation)
		{
			ProcessAddressInterpolation(sector, way);
			continue;
		}

		World* world = LigumX::GetInstance().GetWorld();

		OSMElementType wayType = way->GetOSMElementType();

		bool fillFlag = (wayType == OSMElementType::OSMElementType_Building_Unmarked ||
			wayType == OSMElementType::OSMElementType_Building_School ||
			wayType == OSMElementType::OSMElementType_LeisurePark ||
			wayType == OSMElementType::OSMElementType_NaturalWood ||
			wayType == OSMElementType::OSMElementType_NaturalWater ||
			wayType == OSMElementType::OSMElementType_Landuse);

		if (false && fillFlag)
		{
			Building building(way);
			bool success = building.GenerateModel();

			if (success)
			{
				way->SetFilledIn(true);

				//renderer.AddToDebugModels(building.m_Model);

				Texture* tex = sector->GetGraphicalData()->GetSplatMapTexture();
				lxAssert(tex != nullptr);


				struct TerrainColorEditingJob
				{
					TerrainColorEditingJob() { }

					TerrainColorEditingJob(Sector* sector, glm::ivec2 texelMin, glm::ivec2 texelMax)
						: m_Sector(sector)
						, m_TexelMin(texelMin)
						, m_TexelMax(texelMax)
					{

					}

					void Execute(const std::vector<Triangle>& triangles)
					{
						lxAssert(m_Sector != nullptr);

						Texture* tex = m_Sector->GetGraphicalData()->GetSplatMapTexture();
						unsigned char* val = tex->GetTextureData();

						int numBytes = tex->GetNumBytes();
						int stride = 4;
						int dataOffset = stride * (m_TexelMin.y * tex->GetSize().x + m_TexelMin.x);

						unsigned char* offsetVal = val + dataOffset;
						for (int i = 0; i < m_TexelMax.x - m_TexelMin.x; ++i)
						{
							for (int j = 0; j < m_TexelMax.y - m_TexelMin.y; ++j)
							{
								int index = (int)(stride * (j * tex->GetSize().y + i));

								if (index < 0 || index > numBytes)
								{
									continue;
								}

								glm::ivec2 texelSpace = m_TexelMin + glm::ivec2(i, j);
								glm::vec2 uvSpace = (glm::vec2)texelSpace / (glm::vec2)tex->GetSize();
								glm::vec3 worldSpace = m_Sector->GetWorldPositionForUV(uvSpace);

								for (int t = 0; t < triangles.size(); ++t)
								{
									const Triangle& triangle = triangles[t];

									glm::vec3 offset = glm::vec3(0.f);

									// TODO jpp : make this work eventually...
									//glm::vec3 barycentricCoords = Barycentric(worldSpace, 
									//										  triangle.m_Vertices[0] - offset, 
									//										  triangle.m_Vertices[1] - offset, 
									//										  triangle.m_Vertices[2] - offset);

									//if (glm::all(glm::lessThan(glm::vec3(1, 1, 1), barycentricCoords)))
									//{
									//	offsetVal[index] = (unsigned char)255;
									//	break;
									//}

									if (PointInTriangle(worldSpace,
										triangle.m_Vertices[0] - offset,
										triangle.m_Vertices[1] - offset,
										triangle.m_Vertices[2] - offset))
									{
										offsetVal[index] = (unsigned char)255;
										break;
									}
								}

							}
						}

						tex->UpdateFromData();
					}

					Sector* m_Sector;
					glm::ivec2 m_TexelMin;
					glm::ivec2 m_TexelMax;
				};


				const std::vector<Triangle> triangles = building.GetTriangles();

				glm::vec2 sectorUVMin = sector->GetUVForWorldPosition(building.m_MinCoords);
				glm::vec2 sectorUVMax = sector->GetUVForWorldPosition(building.m_MaxCoords);

				glm::ivec2 texelMin = glm::ivec2(sectorUVMin * glm::vec2(tex->GetSize()));
				glm::ivec2 texelMax = glm::ivec2(sectorUVMax * glm::vec2(tex->GetSize()));

				std::vector<TerrainColorEditingJob> terrainJobs;

				// todo JPP : fix this mess
				// corners bug

				if (texelMin.x < 0)
				{
					TerrainColorEditingJob additionalJob;
					additionalJob.m_TexelMin.x = tex->GetSize().x + texelMin.x;
					additionalJob.m_TexelMax.x = tex->GetSize().x;

					additionalJob.m_TexelMin.y = texelMin.y;
					additionalJob.m_TexelMax.y = texelMax.y;

					texelMin.x = 0;

					glm::ivec2 sectorNormalizedIndex = sector->GetOffsetIndex();
					Sector* thisSector = world->GetSectorByIndex(sectorNormalizedIndex + glm::ivec2(-1, 0));
					additionalJob.m_Sector = thisSector;

					terrainJobs.push_back(additionalJob);
				}

				if (texelMax.x > tex->GetSize().x)
				{
					TerrainColorEditingJob additionalJob;
					additionalJob.m_TexelMin.x = 0;
					additionalJob.m_TexelMax.x = texelMax.x;

					additionalJob.m_TexelMin.y = texelMin.y;
					additionalJob.m_TexelMax.y = texelMax.y;

					texelMax.x = tex->GetSize().x;

					glm::ivec2 sectorNormalizedIndex = sector->GetOffsetIndex();
					Sector* thisSector = world->GetSectorByIndex(sectorNormalizedIndex + glm::ivec2(1, 0));
					additionalJob.m_Sector = thisSector;

					terrainJobs.push_back(additionalJob);
				}

				if (texelMin.y < 0)
				{
					TerrainColorEditingJob additionalJob;
					additionalJob.m_TexelMin.y = tex->GetSize().y + texelMin.y;
					additionalJob.m_TexelMax.y = tex->GetSize().y;

					additionalJob.m_TexelMin.x = texelMin.x;
					additionalJob.m_TexelMax.x = texelMax.x;

					texelMin.y = 0;

					glm::ivec2 sectorNormalizedIndex = sector->GetOffsetIndex();
					Sector* thisSector = world->GetSectorByIndex(sectorNormalizedIndex + glm::ivec2(0, -1));
					additionalJob.m_Sector = thisSector;

					terrainJobs.push_back(additionalJob);
				}

				if (texelMax.y > tex->GetSize().y)
				{
					TerrainColorEditingJob additionalJob;
					additionalJob.m_TexelMin.y = 0;
					additionalJob.m_TexelMax.y = texelMax.y;

					additionalJob.m_TexelMin.x = texelMin.x;
					additionalJob.m_TexelMax.x = texelMax.x;

					texelMax.y = tex->GetSize().y;

					glm::ivec2 sectorNormalizedIndex = sector->GetOffsetIndex();
					Sector* thisSector = world->GetSectorByIndex(sectorNormalizedIndex + glm::ivec2(0, 1));
					additionalJob.m_Sector = thisSector;

					terrainJobs.push_back(additionalJob);
				}

				TerrainColorEditingJob mainJob(sector, texelMin, texelMax);
				terrainJobs.push_back(mainJob);


				for (TerrainColorEditingJob& job : terrainJobs)
				{
					job.Execute(triangles);
				}
			}
		}

	}
}
