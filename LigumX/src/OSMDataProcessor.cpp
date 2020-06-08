#include "OSMDataProcessor.h"
#include "OSMDataProcessorSettings.h"
#include "OSMElementTypeDataStore.h"

#include "CPUBuffers.h"
#include "Mesh.h"
#include "Material.h"
#include "Model.h"
#include "Visual.h"
#include "Renderer.h"
#include "Editor.h"
#include "OSMTool.h"

#include "Entity.h"
#include "OSMElementComponent.h"	
#include "LigumX.h"
#include "EngineSettings.h"
#include "World.h"
#include "Sector.h"
#include "SectorGraphicalData.h"

#include "DefaultTextureHolder.h"

#include "Building.h"

#include "Node.h"
#include "Way.h"

constexpr int g_BrickMaterialID = 17835;

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


#pragma region  CLASS_SOURCE OSMDataProcessor

#include "OSMDataProcessor.h"
#include "serializer.h"
#include "OSMDataProcessorSettings.h"
const ClassPropertyData OSMDataProcessor::g_Properties[] = 
{
{ "Settings", PIDX_Settings, offsetof(OSMDataProcessor, m_Settings), 0, LXType_ObjectPtr, sizeof(OSMDataProcessorSettings*), LXType_OSMDataProcessorSettings, true, LXType_None, false, 0, 0, 0, 0,}, 
};
void OSMDataProcessor::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeObjectPtr(g_Properties[PIDX_Settings], m_Settings);
}
bool OSMDataProcessor::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool OSMDataProcessor::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowObject2(this, g_Properties[PIDX_Settings], &m_Settings  );
	return true;
}
void OSMDataProcessor::Clone(LXObject* otherObj)
{
	super::Clone(otherObj);
	OSMDataProcessor* other = (OSMDataProcessor*) otherObj;
	other->SetSettings(m_Settings);
}
const char* OSMDataProcessor::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE OSMDataProcessor

OSMDataProcessor::OSMDataProcessor()
{
}

void OSMDataProcessor::Reset()
{
	m_RoadsProcessed = 0;
	m_GenericBuildingsProcessed = 0;
	m_AddressInterpolationsProcessed = 0;
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

		m_TexelMax = glm::min(tex->GetSize(), m_TexelMax);
		m_TexelMin = glm::max(glm::ivec2(0, 0), m_TexelMin);

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
						offsetVal[index] = (unsigned char)(m_Color[0] * 255);
						offsetVal[index + 1] = (unsigned char)(m_Color[1] * 255);
						offsetVal[index + 2] = (unsigned char)(m_Color[2] * 255);
						offsetVal[index + 3] = (unsigned char)(m_Color[3] * 255);
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

	bool m_LogJobs = false;
};

bool PointInBuilding(Sector* sector, const glm::vec3& worldSpacePosition)
{
	for (Building& building : sector->GetBuildings())
	{
		//Mesh* mesh = entity->GetModel()->m_meshes[0];

		//int numTriangles = mesh->m_buffers.vertexPositions.size() / 3;
		//for (int i = 0; i < numTriangles; ++i)
		//{
		//	int tIdx = i * 3;
		//	const glm::vec3& v0 = mesh->m_buffers.vertexPositions[tIdx + 0];
		//	const glm::vec3& v1 = mesh->m_buffers.vertexPositions[tIdx + 1];
		//	const glm::vec3& v2 = mesh->m_buffers.vertexPositions[tIdx + 2];
		for (const Triangle& t : building.GetTriangles())
		{
			if (PointInTriangle(worldSpacePosition, t.m_Vertices[0], t.m_Vertices[1], t.m_Vertices[2]))
			{
				return true;
			}
		}
	}

	return false;
}
bool PointInRoad(Sector* sector, const glm::vec3& worldSpacePosition)
{
	for (Entity* entity : sector->GetGraphicalData()->GetRoadEntities())
	{
		Visual* visual = entity->GetComponent<Visual>();
		if (visual)
		{
			Mesh* mesh = visual->GetModel()->GetMeshes()[0];

			int numTriangles = (int)mesh->m_buffers.GetVertexPositions().size() / 3;
			for (int i = 0; i < numTriangles; ++i)
			{
				int tIdx = i * 3;
				const glm::vec3& v0 = mesh->m_buffers.GetVertexPositions()[tIdx + 0];
				const glm::vec3& v1 = mesh->m_buffers.GetVertexPositions()[tIdx + 1];
				const glm::vec3& v2 = mesh->m_buffers.GetVertexPositions()[tIdx + 2];

				if (PointInTriangle(worldSpacePosition, v0, v1, v2))
				{
					return true;
				}
			}
		}
	}

	return false;
}

void Add3DBox(Mesh* mesh, const glm::vec3& start, const glm::vec3& direction, const glm::vec3& back, const glm::vec3& up, const glm::vec3& dimensions)
{
	std::vector<glm::vec2>& uvs = mesh->m_buffers.GetVertexUVs();
	std::vector<glm::vec3>& vertices = mesh->m_buffers.GetVertexPositions();
	std::vector<glm::vec3>& normals = mesh->m_buffers.GetVertexNormals();

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

	

	uvs.push_back(glm::vec2(0, 0));
	uvs.push_back(glm::vec2(0, dimensions.z));
	uvs.push_back(glm::vec2(dimensions.x, 0));
	uvs.push_back(glm::vec2(0, dimensions.z));
	uvs.push_back(glm::vec2(dimensions.x, dimensions.z));
	uvs.push_back(glm::vec2(dimensions.x, 0));

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

		uvs.push_back(glm::vec2(0, 0));
		uvs.push_back(glm::vec2(dimensions.x, 0));
		uvs.push_back(glm::vec2(0, dimensions.x));
		uvs.push_back(glm::vec2(dimensions.x, 0));
		uvs.push_back(glm::vec2(dimensions.x, dimensions.x));
		uvs.push_back(glm::vec2(0, dimensions.x));

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

		uvs.push_back(glm::vec2(0, 0));
		uvs.push_back(glm::vec2(dimensions.x, 0));
		uvs.push_back(glm::vec2(0, dimensions.x));
		uvs.push_back(glm::vec2(dimensions.x, 0));
		uvs.push_back(glm::vec2(dimensions.x, dimensions.x));
		uvs.push_back(glm::vec2(0, dimensions.x));

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

		uvs.push_back(glm::vec2(0, 0));
		uvs.push_back(glm::vec2(dimensions.y, 0));
		uvs.push_back(glm::vec2(0, dimensions.y));
		uvs.push_back(glm::vec2(dimensions.y, 0));
		uvs.push_back(glm::vec2(dimensions.y, dimensions.y));
		uvs.push_back(glm::vec2(0, dimensions.y));

		normal = glm::normalize(glm::cross(back, up));
		normals.push_back(normal);
		normals.push_back(normal);
		normals.push_back(normal);
		normals.push_back(normal);
		normals.push_back(normal);
		normals.push_back(normal);

		numQuads = 4;
	}

}

void OSMDataProcessor::PrepareNextBuilding(AddrInterpBuildingInfo& buildingInfo, const glm::vec3& direction, float& spaceLeft, glm::vec3& plotStart)
{
	float minNeighborDistance = 10.f;
	float maxNeighborDistance = 70.f;
	float neighborDistance = GetRandomValue(minNeighborDistance, maxNeighborDistance);

	// move to next building start
	plotStart += (buildingInfo.GetPlotLength() + neighborDistance) * direction;
	spaceLeft -= neighborDistance;

	buildingInfo = AddrInterpBuildingInfo(m_Settings);

	spaceLeft -= buildingInfo.GetPlotLength();
}


Mesh* OSMDataProcessor::BuildAdressInterpolationBuilding(Way* way, Entity* entity, Mesh*& groundMesh)
{
	glm::vec3 up = glm::vec3(0, 0, 1);

	Mesh* buildingMesh = new Mesh();
	CPUBuffers fullBuffers;

	glm::vec3 centroid = entity->GetPosition();

	for (auto nodeIt = way->GetNodes().begin(); nodeIt != (way->GetNodes().end() - 1); ++nodeIt)
	{
		Node* node = *nodeIt;
		Node* nextNode = *(nodeIt + 1);
		glm::vec3 nodePos = node->GetWorldPosition() - centroid;
		glm::vec3 nodePos2 = nextNode->GetWorldPosition() - centroid;

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
		int buildingIndex = 0;
		while (spaceLeft > m_Settings->GetMinFacadeLength())
		{

			glm::vec3 buildingStart = plotStart + direction * buildingInfo.m_PaddingBeforeFacade;

			//if (m_Settings->GetCheckPointInRoad())
			//{
			//	if (PointInRoad(sector, buildingStart))
			//	{
			//		PrepareNextBuilding(buildingInfo, direction, spaceLeft, plotStart);
			//		break;
			//	}
			//}

			//if (m_Settings->GetCheckPointInBuilding())
			//{
			//	if (PointInBuilding(sector, buildingStart))
			//	{
			//		PrepareNextBuilding(buildingInfo, direction, spaceLeft, plotStart);
			//		break;
			//	}
			//}

			const glm::vec3& dimensions = buildingInfo.m_BuildingDimensions;

			float depthClearanceRatio = 1.5f;
			glm::vec3 back = buildingStart + depthClearanceRatio * right * dimensions.y;

			//if (PointInRoad(sector, back))
			//{
			//	right *= -1;
			//}

			//if (PointInBuilding(sector, back))
			//{
			//	PrepareNextBuilding(buildingInfo, direction, spaceLeft, plotStart);
			//	break;
			//}


			glm::vec3 plotForward = buildingInfo.GetPlotLength() * direction;
			glm::vec3 plotBack = depthClearanceRatio * right * buildingInfo.GetPlotDepth();
			glm::vec3 plotEnd = plotStart + plotForward + plotBack;

#if 0
			glm::vec2 plotUVMin = sector->GetUVForWorldPosition(plotStart);
			glm::vec2 plotUVMax = sector->GetUVForWorldPosition(plotEnd);


			Texture* tex = sector->GetGraphicalData()->GetSplatMapTexture();
			lxAssert(tex != nullptr);

			glm::ivec2 texelMin = glm::ivec2(plotUVMin * glm::vec2(tex->GetSize()));
			glm::ivec2 texelMax = glm::ivec2(plotUVMax * glm::vec2(tex->GetSize()));

			//TerrainColorEditingJob terrainJob(sector, texelMin, texelMax, glm::vec4(0, 72, 120, 255) / 255.f);
			//terrainJob.Execute();
#endif

			Add3DBox(buildingMesh, buildingStart, direction, right, up, dimensions);
	
			glm::vec3 v0 = plotStart;
			glm::vec3 v1 = plotStart + plotForward;
			glm::vec3 v2 = plotEnd;
			glm::vec3 v3 = plotStart + plotBack;

			CPUBuffers groundBuffers;

			groundBuffers.AddTo_VertexPositions(v0);
			groundBuffers.AddTo_VertexPositions(v1);
			groundBuffers.AddTo_VertexPositions(v2);
			groundBuffers.AddTo_VertexPositions(v3);

			groundBuffers.AddTo_VertexNormals(glm::vec3(0, 0, 1));
			groundBuffers.AddTo_VertexNormals(glm::vec3(0, 0, 1));
			groundBuffers.AddTo_VertexNormals(glm::vec3(0, 0, 1));
			groundBuffers.AddTo_VertexNormals(glm::vec3(0,0,1));

			groundBuffers.AddTo_VertexUVs(glm::vec2(0, 0));
			groundBuffers.AddTo_VertexUVs(glm::vec2(0, 1));
			groundBuffers.AddTo_VertexUVs(glm::vec2(1, 1));
			groundBuffers.AddTo_VertexUVs(glm::vec2(1, 0));

			int baseIndex = buildingIndex * 4;
			groundBuffers.AddTo_IndexBuffer(baseIndex + 0);
			groundBuffers.AddTo_IndexBuffer(baseIndex + 1);
			groundBuffers.AddTo_IndexBuffer(baseIndex + 2);
			groundBuffers.AddTo_IndexBuffer(baseIndex + 2);
			groundBuffers.AddTo_IndexBuffer(baseIndex + 3);
			groundBuffers.AddTo_IndexBuffer(baseIndex + 0);

			fullBuffers.AppendBuffer(groundBuffers);

			buildingIndex++;

			PrepareNextBuilding(buildingInfo, direction, spaceLeft, plotStart);
		}

	}

	buildingMesh->CreateBuffers();

	groundMesh = new Mesh(fullBuffers, GL::PrimitiveMode::Triangles, false);

	return buildingMesh;
}


Mesh* OSMDataProcessor::BuildGenericBuilding(Way* way, Entity* entity)
{
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	glm::vec3 up = glm::vec3(0, 0, 1);

	const glm::vec3& centroid = entity->GetPosition();

	float buildingHeight = m_Settings->GetBuildingHeightBase();
	float heightDelta = 2 * ((rand() / (float)RAND_MAX) - 0.5f); // -1 to 1
	heightDelta *= m_Settings->GetBuildingHeightVariance();
	buildingHeight += heightDelta;

	for (auto nodeIt = way->GetNodes().begin(); nodeIt != (way->GetNodes().end() - 1); ++nodeIt)
	{
		Node* node = *nodeIt;
		Node* nextNode = *(nodeIt + 1);
		glm::vec3 nodePos = node->GetWorldPosition() - centroid;
		glm::vec3 nextPos = nextNode->GetWorldPosition() - centroid;

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


//
//void OSMDataProcessor::ProcessGenericBuilding(Sector* sector, Way* way)
//{
//	Mesh* buildingMesh = BuildGenericBuilding(sector, way);
//
//	if (buildingMesh != nullptr)
//	{
//		Renderer& renderer = Renderer::GetInstance();
//
//		Model* buildingModel = new Model();
//		Material* brickWallMaterial = g_ObjectManager->FindObjectByID<Material>(g_BrickMaterialID, true);
//
//		lxAssert(brickWallMaterial);
//
//		buildingModel->addMesh(buildingMesh, brickWallMaterial);
//		buildingModel->SetName("Building_Test");
//
//		Entity* buildingEntity = new Entity();
//		buildingEntity->SetName("Building - " + way->GetName());
//		buildingEntity->SetModel(buildingModel);
//
//		buildingEntity->SetVisible(true);
//
//		buildingEntity->UpdateAABB();
//
//		OSMElementComponent* osmElementComponent = g_ObjectManager->CreateObject<OSMElementComponent>();
//		osmElementComponent->SetWay(way);
//		buildingEntity->AddTo_Components(osmElementComponent);
//
//		//World* world = LigumX::GetInstance().GetWorld();
//		//world->AddTo_Entities(roadEntity);
//
//		sector->GetGraphicalData()->GetStaticEntities().push_back(buildingEntity);
//	}
//}

Model* OSMDataProcessor::CreateBuildingModel(Way* way, Entity* entity)
{
	Mesh* buildingMesh = BuildGenericBuilding(way, entity);

	if (buildingMesh != nullptr)
	{
		Model* buildingModel = new Model();
		Material* buildingMaterial = m_Settings->GetGenericBuildingMaterial();
		buildingModel->addMesh(buildingMesh, buildingMaterial);
		buildingModel->SetName("GenericBuilding_Test");

		return buildingModel;
	}

	return nullptr;
}

Mesh* OSMDataProcessor::BuildRoadMesh(Way* way, Entity* entity)
{
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> vertices;
	glm::vec3 up = glm::vec3(0, 0, 1);

	std::vector<glm::vec3> realNodeWorldPositions;

	float worldScale = g_EngineSettings->GetWorldScale();
	glm::vec2 worldScale2 = glm::vec2(worldScale, worldScale);

	static float heightModifier = 0.001f; // todo jpp handle zfight better
	float baseHeight = m_Settings->GetRoadHeight();
	float meshHeight = baseHeight + heightModifier;

	bool alley = way->hasTag("service");


	glm::vec3 centroid = glm::vec3(0, 0, 0);
	if (entity)
	{
		centroid = entity->GetPosition();
	}

	for (int i = 0; i < way->GetNodes().size(); ++i)
	{
		Node* node = way->GetNodes()[i];
		glm::vec3 nodePos = node->GetWorldPosition();
		nodePos.z = meshHeight; 
		realNodeWorldPositions.push_back(nodePos - centroid);
	}

	heightModifier += 0.002f;

	float roadWidth = alley ? m_Settings->GetAlleyWidth() : m_Settings->GetRoadWidth();

	// missing the first constant part from p0 to n1?
	for (int i = 1; i < realNodeWorldPositions.size(); ++i)
	{
		// 0 (1 2) 3 (4 5) 6 (7 8) 9
		// osm nodes are at i * 3 in nodeMeshWorldPositions
		int nodeIdx = i ;
		int prevNodeIdx = i - 1;

		const glm::vec3& nodePos = realNodeWorldPositions[nodeIdx];
		const glm::vec3& prevPos = realNodeWorldPositions[prevNodeIdx];

		// constant part from n0 to n1
		glm::vec3 n0 = realNodeWorldPositions[prevNodeIdx];
		glm::vec3 n1 = realNodeWorldPositions[prevNodeIdx + 1];

		glm::vec3 pSegment = (n1 - n0);
		glm::vec3 pRight = glm::cross(glm::normalize(pSegment), up);

		float offset = roadWidth / 2.f;

		glm::vec3 p0 = n0 - offset * pRight;
		glm::vec3 p1 = n0 + offset * pRight;

		glm::vec3 p2 = n1 - offset * pRight;
		glm::vec3 p3 = n1 + offset * pRight;

#if 0
		p2		p3
		|  \	|
		|   \	|
		|    \	|
		|	  \ |
		p0------p1
#endif

		vertices.push_back(p0);
		vertices.push_back(p1);
		vertices.push_back(p2);
		vertices.push_back(p1);
		vertices.push_back(p3);
		vertices.push_back(p2);

		lxAssert(p0 != p1);
		lxAssert(p2 != p3);
		lxAssert(p0 != p2);

		glm::vec2 uv0 = glm::vec2(0, 0);
		glm::vec2 uv3 = glm::vec2(1, 1);
		uv3.y *= glm::length(pSegment);

		glm::vec2 uv1 = glm::vec2(uv3.x, uv0.y);
		glm::vec2 uv2 = glm::vec2(uv0.x, uv3.y);

		uvs.push_back(uv0);
		uvs.push_back(uv1);
		uvs.push_back(uv2);
		uvs.push_back(uv1);
		uvs.push_back(uv3);
		uvs.push_back(uv2);

		if (i == realNodeWorldPositions.size() - 1)
		{
#if 0
			p2		p3
			| \		|
			|   \	|
			|    \	|
			|	  \ |
			p0------p1
#endif

			// last quad
			{
				const glm::vec3& n0 = realNodeWorldPositions[realNodeWorldPositions.size() - 2];
				const glm::vec3& n1 = realNodeWorldPositions[realNodeWorldPositions.size() - 1];

				glm::vec3 pSegment = glm::normalize(n1 - n0);
				glm::vec3 pRight = glm::cross(pSegment, up);

				float offset = roadWidth / 2.f;

				glm::vec3 p0 = n0 - offset * pRight;
				glm::vec3 p1 = n0 + offset * pRight;

				glm::vec3 p2 = n1 - offset * pRight;
				glm::vec3 p3 = n1 + offset * pRight;

				vertices.push_back(p0);
				vertices.push_back(p1);
				vertices.push_back(p2);
				vertices.push_back(p1);
				vertices.push_back(p3);
				vertices.push_back(p2);

				lxAssert(p0 != p1);
				lxAssert(p2 != p3);
				lxAssert(p0 != p2);
				glm::vec2 uv0 = glm::vec2(0, 0);
				glm::vec2 uv3 = glm::vec2(1, 1);

				uv3 *= glm::abs(glm::vec2(p3) - glm::vec2(p0));

				glm::vec2 uv1 = glm::vec2(uv3.x, uv0.y);
				glm::vec2 uv2 = glm::vec2(uv0.x, uv3.y);

				uvs.push_back(uv0);
				uvs.push_back(uv1);
				uvs.push_back(uv2);
				uvs.push_back(uv1);
				uvs.push_back(uv3);
				uvs.push_back(uv2);

			}
		}
	}

	if (vertices.size() > 0)
	{
		Mesh* roadMesh = new Mesh(vertices, uvs, GL::PrimitiveMode::Triangles, false);
		return roadMesh;
	}

	return nullptr;
}

Mesh* OSMDataProcessor::ProcessWayNodes(Sector* sector, Way* way)
{
	//std::vector<glm::vec2> uvs;
	//std::vector<glm::vec3> vertices;
	//glm::vec3 up = glm::vec3(0, 0, 1);

	//std::vector<glm::vec3> realNodeWorldPositions;

	//float worldScale = g_EngineSettings->GetWorldScale();
	//glm::vec2 worldScale2 = glm::vec2(worldScale, worldScale);

	//for (auto nodeIt = way->GetNodes().begin(); nodeIt != way->GetNodes().end(); ++nodeIt)
	//{
	//	Node* node = *nodeIt;
	//	const glm::vec2& sectorRelativePosition = node->GetSectorRelativePosition();

	//	glm::vec2 p = sectorRelativePosition - glm::vec2(sector->GetOffsetIndex());
	//	//const glm::vec3& nodePos = glm::vec3(p, 0.f);

	//	glm::vec3 nodePos = node->GetWorldPosition();
	//	realNodeWorldPositions.push_back(nodePos);
	//}


	return nullptr;
}

Model* OSMDataProcessor::CreateRoadModel(Way* way, Entity* entity)
{
	Mesh* roadMesh = BuildRoadMesh(way, entity);

	if (roadMesh != nullptr)
	{
		Model* roadModel = new Model();
		Material* roadMaterial = m_Settings->GetRoadMaterial();

		if (way->hasTag("service"))
		{
			roadMaterial = m_Settings->GetAlleyMaterial();
		}

		roadModel->addMesh(roadMesh, roadMaterial);
		roadModel->SetName("Road_Test");

		glm::vec3 heightDelta = glm::vec3(0, 0, m_Settings->GetRoadHeight());
		entity->AddTo_Position(heightDelta);

		return roadModel;
	}

	return nullptr;
}



void OSMDataProcessor::CreateEntity(Way* way)
{
	Entity* newEntity = new Entity();
	newEntity->SetName(lxFormat("OSMEntity - %s (%lld)", way->GetName().c_str(), way->GetOSMId()));

	OSMElementComponent* osmElementComponent = g_ObjectManager->CreateObject<OSMElementComponent>();
	osmElementComponent->SetWay(way);
	newEntity->AddTo_Components(osmElementComponent);

	glm::vec3 entityPos = glm::vec3(0, 0, 0);
	glm::vec3 entityMin = glm::vec3(LX_LIMITS_FLOAT_MAX);
	glm::vec3 entityMax = glm::vec3(LX_LIMITS_FLOAT_MIN);
	int count = 0;
	for (auto nodeIt = way->GetNodes().begin(); nodeIt != way->GetNodes().end(); ++nodeIt)
	{
		Node* node = *nodeIt;
		const glm::vec3& nodePos = node->GetWorldPosition();
		entityPos += nodePos;
		entityMin = glm::min(nodePos, entityMin);
		entityMin = glm::max(nodePos, entityMax);
		count++;
	}

	entityPos /= count;
	newEntity->SetPosition(entityPos);

	newEntity->SetVisible(true);

	World* world = LigumX::GetInstance().GetWorld();
	world->AddTo_Entities(newEntity);
}

Model* OSMDataProcessor::CreateAddressInterpolationModel(Way* way, Entity* entity)
{
	Mesh* buildingMesh = nullptr;

	Mesh* groundMesh = nullptr;
	buildingMesh = BuildAdressInterpolationBuilding(way, entity, groundMesh);

	int nbBuildings = 5;
	float buildingWidth = 5.f;
	float buildingWOffset = 5.f;;

	if (buildingMesh != nullptr)
	{
		Renderer& renderer = Renderer::GetInstance();

		Model* buildingModel = g_ObjectManager->CreateNewObject<Model>();
		Material* material = m_Settings->GetAdressInterpolationMaterial();

		lxAssert(material);

		buildingModel->addMesh(buildingMesh, material);
		buildingModel->SetName("AddrInterpolation_Test");

		return buildingModel;
	}

#if 0
	if (groundMesh != nullptr)
	{
		Renderer& renderer = Renderer::GetInstance();

		Model* groundModel = new Model();
		Material* groundMaterial = new Material();
		groundMaterial->SetDiffuseTexture(g_Editor->GetDefaultTextureHolder()->GetGrassTexture());

		groundModel->addMesh(groundMesh, groundMaterial);
		groundModel->SetName("AddrInterpolation_Test_Ground");

		Entity* groundEntity = new Entity();

		groundEntity->SetName("OSM_GENERATED_Ground_ADDRINTERP - " + way->GetName());
		Visual* visual = g_ObjectManager->CreateNewObject<Visual>();
		visual->SetModel(groundModel);

		groundEntity->SetVisible(true);
		groundEntity->AddTo_Components(visual);

		OSMElementComponent* osmElementComponent = g_ObjectManager->CreateObject<OSMElementComponent>();
		osmElementComponent->SetWay(way);
		groundEntity->AddTo_Components(osmElementComponent);

		sector->GetGraphicalData()->GetStaticEntities().push_back(groundEntity);
	}
#endif
}

bool OSMDataProcessor::IsRoad(Way* way)
{
	return way->GetOSMElementType() >= OSMElementType_HighwayTrunk && way->GetOSMElementType() <= OSMElementType_HighwayUnclassified;
}

static int m_MaxRoadsToProcess = 9999999;

static int m_MaxGenericBuildingsToProcess = 0;

static int m_MaxAddressInterpolationsToProcess = 0;

static bool m_FillInEnabled = true;



void OSMDataProcessor::ProcessSector(Sector* sector)
{
	for (auto it = sector->m_Data->ways.begin(); it != sector->m_Data->ways.end(); ++it)
	{
		Way* way = it->second;
		if (m_RoadsProcessed < m_MaxRoadsToProcess)
		{
			CreateEntity(way);
			m_RoadsProcessed++;
		}

	}
}

Model* OSMDataProcessor::CreateModelForWay(Way* way, Entity* entity)
{
	std::vector<Way*> addressInterpolationWays;

	bool isRoad = IsRoad(way);
	if (isRoad)
	{
		return CreateRoadModel(way, entity);
	}

	bool isGenericBuilding = g_OSMElementTypeDataStore->GetData()[way->GetOSMElementType()]->GetIsBuilding();
	if (isGenericBuilding)
	{
		return CreateBuildingModel(way, entity);
	}


	bool isAddressInterpolation = way->GetOSMElementType() == OSMElementType_AddressInterpolation;
	if (isAddressInterpolation)
	{
		return CreateAddressInterpolationModel(way, entity);
	}

	OSMElementType wayType = way->GetOSMElementType();

	bool fillIn = g_OSMElementTypeDataStore->GetData()[wayType]->GetFillIn();

	if (fillIn && m_FillInEnabled)
	{
		Building building(way);

		Material* material = m_Settings->GetGrassMaterial();

		if (way->IsParking())
		{
			material = m_Settings->GetParkingMaterial();
		}
		else if (way->IsBareRock())
		{
			material = m_Settings->GetRockMaterial();
		}

		bool success = building.GenerateModel(entity, material);

		if (success)
		{
			way->SetFilledIn(true);

			static float heightModifier = 0.001f; // todo jpp handle zfight better
			float height = m_Settings->GetFillInHeight();
			height += heightModifier;
			heightModifier += 0.002f;

			glm::vec3 heightDelta = glm::vec3(0, 0, height);
			entity->AddTo_Position(heightDelta);

			return building.m_Model;
		}
	}

	return nullptr;
}
