#include "OSMDataProcessor.h"
#include "OSMDataProcessorSettings.h"

#include "Mesh.h"
#include "Material.h"
#include "Model.h"
#include "Renderer.h"
#include "Editor.h"
#include "OSMTool.h"

#include "Entity.h"
#include "OSMElementComponent.h"	
#include "LigumX.h"
#include "World.h"
#include "Sector.h"
#include "SectorGraphicalData.h"

#include "Building.h"

#include "Node.h"
#include "Way.h"

constexpr int g_BrickMaterialID = 17835;

#pragma region  CLASS_SOURCE OSMDataProcessor
OSMDataProcessor* g_OSMDataProcessor;

#include "OSMDataProcessor.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData OSMDataProcessor::g_Properties[] = 
{
{ "ObjectID", PIDX_ObjectID, offsetof(OSMDataProcessor, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, 0,}, 
{ "Name", PIDX_Name, offsetof(OSMDataProcessor, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, 0,}, 
{ "RoadWidth", PIDX_RoadWidth, offsetof(OSMDataProcessor, m_RoadWidth), 0, LXType_float, false, LXType_None, 0, 0, 0, 0,}, 
{ "Settings", PIDX_Settings, offsetof(OSMDataProcessor, m_Settings), 0, LXType_OSMDataProcessorSettings, true, LXType_None, 0, 0, 0, 0,}, 
};
bool OSMDataProcessor::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}

#pragma endregion  CLASS_SOURCE OSMDataProcessor

OSMDataProcessor::OSMDataProcessor()
{
	m_Settings = g_ObjectManager->CreateObject<OSMDataProcessorSettings>();
}


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
struct TerrainColorEditingJob
{
	TerrainColorEditingJob() { }

	TerrainColorEditingJob(Sector* sector, glm::ivec2 texelMin, glm::ivec2 texelMax, const glm::vec4& color)
		: m_Sector(sector)
		, m_TexelMin(texelMin)
		, m_TexelMax(texelMax)
		, m_Color (color)
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

	void Execute()
	{
		lxAssert(m_Sector != nullptr);

		Texture* tex = m_Sector->GetGraphicalData()->GetSplatMapTexture();
		unsigned char* val = tex->GetTextureData();

		glm::ivec2 texelMin = glm::min(m_TexelMin, m_TexelMax);
		int numBytes = tex->GetNumBytes();
		int stride = 4;
		int dataOffset = stride * (texelMin.y * tex->GetSize().x + texelMin.x);

		unsigned char* offsetVal = val + dataOffset;

		for (int i = 0; i < abs(m_TexelMax.x - m_TexelMin.x); ++i)
		{
			for (int j = 0; j < abs(m_TexelMax.y - m_TexelMin.y); ++j)
			{
				int index = (int)(stride * (j * tex->GetSize().y + i));

				if (index < 0 || index > numBytes)
				{
					continue;
				}

				offsetVal[index]   = (unsigned char) (m_Color[0] * 255);
				offsetVal[index+1] = (unsigned char) (m_Color[1] * 255);
				offsetVal[index+2] = (unsigned char) (m_Color[2] * 255);
				offsetVal[index+3] = (unsigned char) (m_Color[3] * 255);
			}
		}

		tex->UpdateFromData();


		if (m_LogJobs)
		{
			static int s_NumTerrainJobs;
			std::stringstream ss;
			ss << "Terrain job " << s_NumTerrainJobs << std::endl;
			ss << "Sector : "	<< glm::to_string(m_Sector->GetOffsetIndex()) << std::endl;
			ss << "texelMin : " << glm::to_string(texelMin) << std::endl;
			ss << "m_TexelMax : " << glm::to_string(m_TexelMax) << std::endl;
			ss << "m_Color : " << glm::to_string(m_Color) << std::endl;
			ss << std::endl;

			s_NumTerrainJobs++;

			std::cout << ss.str();
		}
	}

	Sector* m_Sector;
	glm::ivec2 m_TexelMin;
	glm::ivec2 m_TexelMax;

	glm::vec4 m_Color;

	bool m_LogJobs = true;
};


bool PointInRoad(Sector* sector, const glm::vec3& worldSpacePosition)
{
	for (Entity* entity : sector->GetGraphicalData()->GetRoadEntities())
	{
		Mesh* mesh = entity->GetModel()->m_meshes[0];

		int numTriangles = mesh->m_buffers.vertexPositions.size() / 3;
		for (int i = 0; i < numTriangles; ++i)
		{
			int tIdx = i * 3;
			const glm::vec3& v0 = mesh->m_buffers.vertexPositions[tIdx + 0];
			const glm::vec3& v1 = mesh->m_buffers.vertexPositions[tIdx + 1];
			const glm::vec3& v2 = mesh->m_buffers.vertexPositions[tIdx + 2];

			if (PointInTriangle(worldSpacePosition, v0, v1, v2))
			{
				return true;
			}
		}
	}

	return false;
}

void Add3DBox(Mesh* mesh, const glm::vec3& start, const glm::vec3& direction, const glm::vec3& back, const glm::vec3& up, const glm::vec3& dimensions)
{
	std::vector<glm::vec2>& uvs = mesh->m_buffers.m_vertexUVs;
	std::vector<glm::vec3>& vertices = mesh->m_buffers.vertexPositions;
	std::vector<glm::vec3>& normals = mesh->m_buffers.m_vertexNormals;

	glm::vec3 buildingHeight = up * dimensions.z;
	glm::vec3 buildingLength = direction * dimensions.x;
	glm::vec3 buildingDepth = back * dimensions.y;

	glm::vec3 facade00 = start;
	glm::vec3 facade01 = start + buildingHeight;

	glm::vec3 facade10 = start + buildingLength;
	glm::vec3 facade11 = facade10 + buildingHeight;

	glm::vec3 back00 = facade00 + buildingDepth;
	glm::vec3 back01 = back00 + buildingHeight;

	glm::vec3 back10 = back00 + buildingLength;
	glm::vec3 back11 = back10 + buildingHeight;

	vertices.push_back(facade00);
	vertices.push_back(facade01);
	vertices.push_back(facade10);
	vertices.push_back(facade01);
	vertices.push_back(facade11);
	vertices.push_back(facade10);

	glm::vec3 normal = glm::normalize(glm::cross(up, direction));
	normals.push_back(normal);
	normals.push_back(normal);
	normals.push_back(normal);
	normals.push_back(normal);
	normals.push_back(normal);
	normals.push_back(normal);

	int numQuads = 1;
	static volatile bool g_Debug_GenerateDepth = true;
	if (g_Debug_GenerateDepth)
	{
		vertices.push_back(facade10);
		vertices.push_back(facade11);
		vertices.push_back(back10);
		vertices.push_back(facade11);
		vertices.push_back(back11);
		vertices.push_back(back10);

		normal = glm::normalize(glm::cross(up, back));
		normals.push_back(normal);
		normals.push_back(normal);
		normals.push_back(normal);
		normals.push_back(normal);
		normals.push_back(normal);
		normals.push_back(normal);

		vertices.push_back(back10);
		vertices.push_back(back11);
		vertices.push_back(back00);
		vertices.push_back(back11);
		vertices.push_back(back01);
		vertices.push_back(back00);

		normal = glm::normalize(glm::cross(-direction, up));
		normals.push_back(normal);
		normals.push_back(normal);
		normals.push_back(normal);
		normals.push_back(normal);
		normals.push_back(normal);
		normals.push_back(normal);

		vertices.push_back(back00);
		vertices.push_back(back01);
		vertices.push_back(facade00);
		vertices.push_back(back01);
		vertices.push_back(facade01);
		vertices.push_back(facade00);

		normal = glm::normalize(glm::cross(back, up));
		normals.push_back(normal);
		normals.push_back(normal);
		normals.push_back(normal);
		normals.push_back(normal);
		normals.push_back(normal);
		normals.push_back(normal);

		numQuads = 4;
	}

	for (int i = 0; i < numQuads; ++i)
	{
		uvs.push_back(glm::vec2(0, 0));
		uvs.push_back(glm::vec2(1, 0));
		uvs.push_back(glm::vec2(0, 1));
		uvs.push_back(glm::vec2(1, 0));
		uvs.push_back(glm::vec2(1, 1));
		uvs.push_back(glm::vec2(0, 1));
	}
}

float Random0To1()
{
	static float fRandMax = (float)RAND_MAX;
	return rand() / fRandMax;
}

template <typename T>
T GetRandomValue(T min, T max)
{
	return Random0To1() * (max - min) + min;
}

struct AddrInterpBuildingInfo
{
	AddrInterpBuildingInfo(OSMDataProcessorSettings* settings)
	{
		m_BuildingDimensions.x = GetRandomValue(settings->GetMinFacadeLength(), settings->GetMaxFacadeLength());

		float minPlotLength = settings->GetMinPlotLengthRatio() * m_BuildingDimensions.x;
		float maxPlotLength = settings->GetMaxPlotLengthRatio() * m_BuildingDimensions.x;
		m_PlotDimensions.x = GetRandomValue(minPlotLength, maxPlotLength);

		m_PaddingBeforeFacade = (m_PlotDimensions.x - m_BuildingDimensions.x) / 2.f;
		m_PaddingAfterFacade = m_PaddingBeforeFacade;

		m_BuildingDimensions.y = GetRandomValue(settings->GetMinBuildingDepth(), settings->GetMaxBuildingDepth());

		m_PlotDimensions.y = m_BuildingDimensions.y * 1.5f;

		m_BuildingDimensions.z = GetRandomValue(settings->GetMinHeight(), settings->GetMaxHeight());

	}

	float GetPlotLength()		{ return m_PlotDimensions.x; }
	float GetPlotDepth()		{ return m_PlotDimensions.y; }
	float GetBuildingLength()	{ return m_BuildingDimensions.x; }
	float GetBuildingDepth()	{ return m_BuildingDimensions.y; }
	float GetBuildingHeight()	{ return m_BuildingDimensions.z; }

	glm::vec3 m_BuildingDimensions; // (facade, depth, height)
	glm::vec2 m_PlotDimensions; // facade, depth
	float m_PaddingAfterFacade;
	float m_PaddingBeforeFacade;

};

Mesh* OSMDataProcessor::BuildAdressInterpolationBuilding(Sector* sector, Way* way)
{
	glm::vec3 up = glm::vec3(0, 0, 1);

	Mesh* buildingMesh = new Mesh();
	
	for (auto nodeIt = way->GetNodes().begin(); nodeIt != (way->GetNodes().end() - 1); ++nodeIt)
	{
		Node* node = *nodeIt;
		Node* nextNode = *(nodeIt + 1);
		glm::vec3 nodePos = node->GetWorldPosition();
		glm::vec3 nodePos2 = nextNode->GetWorldPosition();

		glm::vec3 wholeSegment = nodePos2 - nodePos;

		float distance = glm::length(wholeSegment);

		if (distance == 0.f)
		{
			continue;
		}

		float spaceLeft = distance;

		AddrInterpBuildingInfo buildingInfo(m_Settings);
		spaceLeft -= buildingInfo.GetPlotLength();;

		glm::vec3 direction = glm::normalize(wholeSegment);
		glm::vec3 right = glm::cross(direction, up);

		glm::vec3 plotStart = nodePos;
		while (spaceLeft > 0)
		{

			glm::vec3 buildingStart = plotStart + direction * buildingInfo.m_PaddingBeforeFacade;

			if (PointInRoad(sector, buildingStart))
			{
				break;
			}

			const glm::vec3& dimensions = buildingInfo.m_BuildingDimensions;

			float depthClearanceRatio = 1.2f;
			glm::vec3 back = buildingStart + depthClearanceRatio * right * dimensions.y;

			if (PointInRoad(sector, back))
			{
				right *= -1;
			}

			Texture* tex = sector->GetGraphicalData()->GetSplatMapTexture();
			lxAssert(tex != nullptr);

			glm::vec3 plotEnd = plotStart + buildingInfo.GetPlotLength() * direction + depthClearanceRatio * right * buildingInfo.GetPlotDepth();

			glm::vec2 plotUVMin = sector->GetUVForWorldPosition(plotStart);
			glm::vec2 plotUVMax = sector->GetUVForWorldPosition(plotEnd);

			glm::ivec2 texelMin = glm::ivec2(plotUVMin * glm::vec2(tex->GetSize()));
			glm::ivec2 texelMax = glm::ivec2(plotUVMax * glm::vec2(tex->GetSize()));

			TerrainColorEditingJob terrainJob(sector, texelMin, texelMax, glm::vec4(0, 1, 0, 1));
			terrainJob.Execute();

			Add3DBox(buildingMesh, buildingStart, direction, right, up, dimensions);

			float minNeighborDistance = 40.f;
			float maxNeighborDistance = 70.f;
			float neighborDistance = GetRandomValue(minNeighborDistance, maxNeighborDistance);

			// move to next building start
			plotStart += (buildingInfo.GetPlotLength() + neighborDistance) * direction;
			spaceLeft -= neighborDistance;

			buildingInfo = AddrInterpBuildingInfo(m_Settings);

			spaceLeft -= buildingInfo.GetPlotLength();
		}

	}

	buildingMesh->CreateBuffers();

	return buildingMesh;
}


Mesh* OSMDataProcessor::BuildGenericBuilding(Sector* sector, Way* way)
{
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
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
		Material* brickWallMaterial = g_ObjectManager->FindObjectByID<Material>(g_BrickMaterialID);

		lxAssert(brickWallMaterial);

		buildingModel->addMesh(buildingMesh, brickWallMaterial);
		buildingModel->SetName("Building_Test");

		Entity* buildingEntity = new Entity();
		buildingEntity->SetName("Building - " + way->GetName());
		buildingEntity->SetModel(buildingModel);

		buildingEntity->SetVisible(true);

		buildingEntity->UpdateAABB();

		OSMElementComponent* osmElementComponent = g_ObjectManager->CreateObject<OSMElementComponent>();
		osmElementComponent->SetWay(way);
		buildingEntity->AddTo_Components(osmElementComponent);

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

		OSMElementComponent* osmElementComponent = g_ObjectManager->CreateObject<OSMElementComponent>();
		osmElementComponent->SetWay(way);
		roadEntity->AddTo_Components(osmElementComponent);

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
		Material* brickWallMaterial = g_ObjectManager->FindObjectByID<Material>(g_BrickMaterialID);

		lxAssert(brickWallMaterial);

		buildingModel->addMesh(buildingMesh, brickWallMaterial);
		buildingModel->SetName("AddrInterpolation_Test");


		Entity* buildingEntity = new Entity();
		buildingEntity->SetName("OSM_GENERATED_ADDRINTERP - " + way->GetName());
		buildingEntity->SetModel(buildingModel);

		buildingEntity->SetVisible(true);

		OSMElementComponent* osmElementComponent = g_ObjectManager->CreateObject<OSMElementComponent>();
		osmElementComponent->SetWay(way);
		buildingEntity->AddTo_Components(osmElementComponent);
		//World* world = LigumX::GetInstance().GetWorld();
		//world->AddTo_Entities(roadEntity);

		sector->GetGraphicalData()->GetStaticEntities().push_back(buildingEntity);
	}

}

bool OSMDataProcessor::IsRoad(Way* way)
{
	return way->GetOSMElementType() >= OSMElementType_HighwayTrunk && way->GetOSMElementType() <= OSMElementType_HighwayUnclassified;
}


void OSMDataProcessor::ProcessSector(Sector* sector)
{
	std::vector<Way*> addressInterpolationWays;

	for (auto it = sector->m_Data->ways.begin(); it != sector->m_Data->ways.end(); ++it)
	{
		Way* way = it->second;

		bool hsNodes = (way->GetNodes().size() > 0);

		bool isRoad = IsRoad(way);
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
			// process them at the end when we have all our roads
			addressInterpolationWays.push_back(way);
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

		if (fillFlag)
		{
			Building building(way);
			bool success = building.GenerateModel();

			if (success)
			{
				way->SetFilledIn(true);

				//renderer.AddToDebugModels(building.m_Model);

				Texture* tex = sector->GetGraphicalData()->GetSplatMapTexture();
				lxAssert(tex != nullptr);

				const std::vector<Triangle> triangles = building.GetTriangles();

				glm::vec2 sectorUVMin = sector->GetUVForWorldPosition(building.m_MinCoords);
				glm::vec2 sectorUVMax = sector->GetUVForWorldPosition(building.m_MaxCoords);

				glm::ivec2 texelMin = glm::ivec2(sectorUVMin * glm::vec2(tex->GetSize()));
				glm::ivec2 texelMax = glm::ivec2(sectorUVMax * glm::vec2(tex->GetSize()));

				std::vector<TerrainColorEditingJob> terrainJobs;

				// todo JPP : fix this mess
				// corners bug
#if 0
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
#endif
				glm::vec4 color = glm::vec4(g_Editor->GetOSMTool()->GetWayDebugColors()[way->GetOSMElementType()], 1.f);
				TerrainColorEditingJob mainJob(sector, texelMin, texelMax, glm::vec4(color));
				terrainJobs.push_back(mainJob);

				for (TerrainColorEditingJob& job : terrainJobs)
				{
					job.Execute(triangles);
				}
			}
		}

	}

	if (m_Settings->GetProcessAddressInterpolation())
	{
		for (Way* addrInterpWay : addressInterpolationWays)
		{
			ProcessAddressInterpolation(sector, addrInterpWay);
		}
	}
	
}
