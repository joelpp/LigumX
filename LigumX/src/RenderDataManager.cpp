#include "stdafx.h"

#include "CullingOptions.h"
#include "RenderingStats.h"

#include "BoundingBoxComponent.h"
#include "RenderDataManager.h"
#include "InputHandler.h"
#include "LigumX.h"
#include "StringUtils.h"
#include "Editor.h"
#include "PickingTool.h"
#include "EngineSettings.h"
#include "Logging.h"
#include "Renderer.h"

#include "Way.h"
#include "Node.h"

#include "Sector.h"
#include "World.h"
#include "SectorData.h"
#include "SectorGraphicalData.h"
#include "Heightfield.h"

#include "Camera.h"
#include "Mesh.h"
#include "Model.h"
#include "Material.h"

#include "glm/gtx/transform.hpp"

#include "Building.h"
RenderDataManager* g_RenderDataManager;
#pragma region  CLASS_SOURCE RenderDataManager

#include "RenderDataManager.h"
#include "CullingOptions.h"
#include "RenderingStats.h"
const ClassPropertyData RenderDataManager::g_Properties[] = 
{
{ "CullingOptions", PIDX_CullingOptions, offsetof(RenderDataManager, m_CullingOptions), 0, LXType_ObjectPtr, sizeof(CullingOptions*), LXType_CullingOptions, true, LXType_None, false, 0, 0, 0, 0,}, 
{ "RenderingStats", PIDX_RenderingStats, offsetof(RenderDataManager, m_RenderingStats), 0, LXType_ObjectPtr, sizeof(RenderingStats*), LXType_RenderingStats, true, LXType_None, false, 0, 0, 0, 0,}, 
};
bool RenderDataManager::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool RenderDataManager::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	LXIMGUI_SHOW_OBJECTREF("CullingOptions", m_CullingOptions);
	LXIMGUI_SHOW_OBJECTREF("RenderingStats", m_RenderingStats);
	return true;
}
const char* RenderDataManager::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE RenderDataManager

using namespace std;
using namespace glm;


FlatWaysMesh::FlatWaysMesh(const std::vector<glm::vec3>& vertices, const std::vector<WayData>& dataBuffer, GL::PrimitiveMode primitiveMode, bool usePointRendering)
{
	m_buffers.m_VertexPositions = vertices;

	m_DataBuffer = dataBuffer;
	m_PrimitiveMode = primitiveMode;
	SetPointRendering(usePointRendering);

	SetUsesIndexBuffer(false);

	padBuffer(VERTEX_UVS);

	CreateBuffers();
}

void FlatWaysMesh::CreateBuffers()
{
	// TODO: I'm not quite convinced this belongs here. or does it?
	LigumX::GetInstance().m_Renderer->createGLBuffer(GL_ARRAY_BUFFER, GetGPUBuffers().glidPositions, m_buffers.m_VertexPositions);

	LigumX::GetInstance().m_Renderer->createGLBuffer(GL_ARRAY_BUFFER, GetGPUBuffers().glidWayDataBuffer, m_DataBuffer);

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, GetGPUBuffers().glidPositions);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, GetGPUBuffers().glidWayDataBuffer);
	glVertexAttribIPointer(3, 2, GL_INT, 0, NULL);
}

ColoredPointsMesh::ColoredPointsMesh(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& dataBuffer, GL::PrimitiveMode primitiveMode, bool usePointRendering)
{
	m_buffers.m_VertexPositions = vertices;

	m_ColorBuffer = dataBuffer;
	m_PrimitiveMode = primitiveMode;
	SetPointRendering(usePointRendering);

	SetUsesIndexBuffer(false);

	CreateBuffers();
}

void ColoredPointsMesh::CreateBuffers()
{
	// TODO: I'm not quite convinced this belongs here. or does it?
	LigumX::GetInstance().m_Renderer->createGLBuffer(GL_ARRAY_BUFFER, GetGPUBuffers().glidPositions, m_buffers.m_VertexPositions);
	LigumX::GetInstance().m_Renderer->createGLBuffer(GL_ARRAY_BUFFER, GetGPUBuffers().glidColorBuffer, m_ColorBuffer);

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, GetGPUBuffers().glidPositions);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, GetGPUBuffers().glidColorBuffer);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}


int findSetBit(int number)
{
    unsigned i = 1, pos = 1;
 
    // Iterate through bits of n till we find a set bit
    // i&n will be non-zero only when 'i' and 'n' have a set bit
    // at same position
    while (!(i & number))
    {
        // Unset current bit and set the next bit in 'i'
        i = i << 1;
 
        // increment position
        ++pos;
    }
 
    return pos;
}   

RenderDataManager::RenderDataManager()
{
	g_RenderDataManager = this;

    nbRoads = 0;
    m_renderingScale = Settings::GetInstance().f("RenderingScale");
    Renderer& renderer = Renderer::GetInstance();

    screenQuadPos.push_back(vec2(1,-1));
    screenQuadPos.push_back(vec2(1,1));
    screenQuadPos.push_back(vec2(-1,-1));
    screenQuadPos.push_back(vec2(-1,1));

    screenQuadTexCoords.push_back(vec2(1,0));
    screenQuadTexCoords.push_back(vec2(1,1));
    screenQuadTexCoords.push_back(vec2(0,0));
    screenQuadTexCoords.push_back(vec2(0,1));

	Renderer::createGLBuffer(renderer.glidScreenQuadPositions, screenQuadPos);
    Renderer::createGLBuffer(renderer.glidScreenQuadTexCoords, screenQuadTexCoords);

	m_VisibleEntities.reserve(m_MaxVisibleEntities);

	m_CullingOptions = g_ObjectManager->CreateObject<CullingOptions>();
	m_RenderingStats = g_ObjectManager->CreateObject<RenderingStats>();
}

void RenderDataManager::addToTerrainBuffer(Sector* newSector)
{
    LigumX& game = LigumX::GetInstance();
    Renderer& renderer = Renderer::GetInstance();
    int counter = 0;

    Heightfield* heightField = newSector->GetHeightfield();


}

Model* RenderDataManager::CreateDebugModel(const std::vector<glm::vec3>& lineData, glm::vec3 color, const char* name)
{
	Renderer& renderer = Renderer::GetInstance();

	Mesh* mesh = new Mesh(lineData, GL::PrimitiveMode::Lines);

	Model* model = new Model();
	model->addMesh(mesh, new Material(renderer.pPipelineLines, color));
	model->SetName(name);
	
	return model;
}

//Model* RenderDataManager::CreateDebugModel(const std::vector<glm::vec3>& lineData, const std::vector<int>& indexBuffer, glm::vec3 color, const char* name)
//{
//	Renderer& renderer = Renderer::GetInstance();
//
//	Mesh* mesh = new Mesh(lineData, indexBuffer, GL_LINES, false);
//
//	Model* model = new Model();
//	model->addMesh(mesh, new Material(renderer.pPipelineLines, color));
//	model->SetName(name);
//
//	return model;
//}


Model* RenderDataManager::CreateFlatDebugModel(const std::vector<glm::vec3>& lineData, const std::vector<WayData>& typeBuffer, glm::vec3 color, const char* name)
{
	Renderer& renderer = Renderer::GetInstance();

	FlatWaysMesh* mesh = new FlatWaysMesh(lineData, typeBuffer, GL::PrimitiveMode::Lines, false);

	Model* model = new Model();
	model->addMesh(mesh, new Material(renderer.pPipelineLines, color));
	model->SetName(name);

	return model;
}


void RenderDataManager::AddDebugModel(const std::vector<glm::vec3>& line, glm::vec3 color)
{
	Renderer& renderer = Renderer::GetInstance();

	Mesh* linesMesh = new Mesh(line, GL::PrimitiveMode::Lines);

	Model* linesModel = new Model();
	linesModel->addMesh(linesMesh, new Material(renderer.pPipelineLines, color));
	linesModel->SetName("Sector_lines_");

	renderer.AddToDebugModels(linesModel);
}

void AddPoint(std::vector<glm::vec3>& points, glm::vec3 point)
{
	if (points.size() > 1)
	{
		points.push_back(points.back());
	}

	points.push_back(point);
}

// Compute barycentric coordinates (u, v, w) for
// point p with respect to triangle (a, b, c)
glm::vec3 Barycentric(const glm::vec3& p, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
{
	glm::vec3 v0 = b - a, v1 = c - a, v2 = p - a;
	float d00 = glm::dot(v0, v0);
	float d01 = glm::dot(v0, v1);
	float d11 = glm::dot(v1, v1);
	float d20 = glm::dot(v2, v0);
	float d21 = glm::dot(v2, v1);
	float denom = d00 * d11 - d01 * d01;
	float v = (d11 * d20 - d01 * d21) / denom;
	float w = (d00 * d21 - d01 * d20) / denom;
	float u = 1.0f - v - w; 
	return glm::vec3(v, w, u);
}


void RenderDataManager::CreateWaysLines(Sector* sector)
{
	SectorGraphicalData* gfxData = sector->GetGraphicalData();

	std::vector<glm::vec3> nodePositions;
	std::vector<glm::vec3> nodeColors;

	World* world = LigumX::GetInstance().GetWorld();
	Renderer& renderer = Renderer::GetInstance();

	Model* waysModel = nullptr;

	std::vector<glm::vec3> flatWayPositions;
	 //todo : make it work with an index buffer someday
	std::vector<WayData> vertexData;

	// todo : make it work with an index buffer someday
	//std::vector<int> indexBuffer;
	int index = 0;
	int previousLastIndex = 0;
	int wayIndex = 0;
	for (auto it = sector->m_Data->ways.begin(); it != sector->m_Data->ways.end(); ++it)
	{
		std::vector<glm::vec3> line;
		Way* way = it->second;
		std::vector<Node*>& nodes = way->GetNodes();

		bool newWay = true;
		for (int i = 0; i < nodes.size(); ++i)
		{
			Node* node = nodes[i];


			const glm::vec3& pos = node->GetWorldPosition();

			nodePositions.push_back(node->GetWorldPosition());
			nodeColors.push_back(glm::vec3(1, 0, 0));

			AddPoint(line, pos);

			// if needed copy last node to gfx 2dline/data
			if (!newWay && index > (previousLastIndex) && (index - 1) != previousLastIndex)
			{
				flatWayPositions.push_back(flatWayPositions.back());
				vertexData.push_back({ way->GetOSMElementType(), way->GetIndexInSector() });
			}


			flatWayPositions.push_back(pos);
			vertexData.push_back({ way->GetOSMElementType(), way->GetIndexInSector() });

			newWay = false;
			index++;

#if 1
			if (i != nodes.size() - 1)
			{
				Node* nextNode = nodes[i + 1];

				float heightSlope = nextNode->GetWorldPosition().z - pos.z;

				const glm::vec3& nextPos = nextNode->GetWorldPosition();

				glm::vec3 distance = nextPos - pos;

				if (distance != glm::vec3(0, 0, 0))
				{
					glm::vec3 direction = glm::normalize(distance);
					float length = glm::length(distance);
					int numsubdivisions = (int)(length / g_EngineSettings->GetWayTessellationFactor());
					float factor = numsubdivisions + 1;
					for (int j = 0; j < numsubdivisions; ++j)
					{
						glm::vec3 subdivisionPoint;

						float lerpFactor = (float)(j + 1) / factor;
						subdivisionPoint = pos + lerpFactor * direction * length ;

						//subdivisionPoint.z = world->SampleHeight(subdivisionPoint);
						//subdivisionPoint.z = lerpFactor * heightSlope;

						nodePositions.push_back(subdivisionPoint);
						nodeColors.push_back(glm::vec3(1, 1, 1));
						AddPoint(line, subdivisionPoint);

						if (!newWay && index >(previousLastIndex) && (index - 1) != previousLastIndex)
						{
							flatWayPositions.push_back(flatWayPositions.back());
							vertexData.push_back({ way->GetOSMElementType(), way->GetIndexInSector() });
						}


						flatWayPositions.push_back(subdivisionPoint);
						vertexData.push_back({ way->GetOSMElementType(), way->GetIndexInSector() });

						newWay = false;
						index++;
					}
				}
			}

#endif

		}
		previousLastIndex = index - 1;
		index -= 1;

		waysModel = CreateDebugModel(line, glm::vec3(1,0,0), "Sector_Lines_");

		gfxData->AddTo_WaysModelsVector(waysModel);
		wayIndex++;

	}


	ColoredPointsMesh* nodeMesh = new ColoredPointsMesh(nodePositions, nodeColors, GL::PrimitiveMode::Points, true);

	Model* nodeModel = new Model();
	nodeModel->addMesh(nodeMesh, new Material(renderer.pPipelineNodes, glm::vec3(1, 1, 1)));
	nodeModel->SetName("Sector_nodes_");

	Model* flatWaysModel = CreateFlatDebugModel(flatWayPositions, vertexData, glm::vec3(1,1,1), "Sector_FlatLines_");

	gfxData->SetNodesModel(nodeModel);
	gfxData->SetWaysModel(flatWaysModel);

}



void RenderDataManager::InitializeSector(Sector* sector)
{
	const glm::ivec2& baseOffset = sector->GetOffsetIndex();

	float offset = g_EngineSettings->GetWorldScale();
	glm::vec3 base = glm::vec3(offset * baseOffset.x, offset * baseOffset.y, 1.f);

	std::vector<glm::vec3> points;
	AddPoint(points, base);
    AddPoint(points, base + glm::vec3(offset,	0,		0));
    AddPoint(points, base + glm::vec3(offset,	offset, 0));
    AddPoint(points, base + glm::vec3(0,		offset, 0));
    AddPoint(points, base);

	glm::vec3 color;

	if (sector->GetDataLoaded())
	{
		color = glm::vec3(0, 0, 1);
	}
	else
	{
		color = glm::vec3(0, 1, 0);
	}

	AddDebugModel(points, color);
}

void RenderDataManager::AddAABBJob(const AABB& aabb, const glm::vec3& color)
{
	AABBJob job;
	job.m_AABB = aabb;
	job.m_Color = color;

	m_AABBJobs.push_back(job);
}


void RenderDataManager::AddAABBJobCentered(const glm::vec3& worldPosition, int brushWidth, const glm::vec3& color)
{
	AddAABBJob(worldPosition - glm::vec3(brushWidth / 2), brushWidth, color);
}

void RenderDataManager::AddAABBJob(const glm::vec3& worldPosition, int brushWidth, const glm::vec3& color)
{
	AABB aabb;
	aabb.SetStartPoint(worldPosition);
	aabb.SetScale(glm::vec3(brushWidth));

	AABBJob job;
	job.m_AABB = aabb;
	job.m_Color = color;

	m_AABBJobs.push_back(job);
}

void RenderDataManager::AddEntityAABB(Entity* entity)
{
	if (!entity)
	{
		return;
	}

	auto it = std::find(m_EntityAABBJobs.begin(), m_EntityAABBJobs.end(), entity);

	if (it == m_EntityAABBJobs.end())
	{
		m_EntityAABBJobs.push_back(entity);
	}
}


std::vector<AABBJob>& RenderDataManager::GetAABBJobs()
{
	return m_AABBJobs;
}

std::vector<Entity*>& RenderDataManager::GetEntityAABBJobs()
{
	return m_EntityAABBJobs;
}

void RenderDataManager::ClearAABBJobs()
{
	m_AABBJobs.clear();
}

void RenderDataManager::AddTimedMessage(const std::string& message)
{
	AddTimedMessage(message, g_EngineSettings->GetMessagesDefaultFrameCount());
}

void RenderDataManager::AddTimedMessage(const std::string& message, int numFrames)
{
	m_TimedMessages.push_back(TimedMessage(message, numFrames));
}

void RenderDataManager::Add2DMessage(const std::string& message, const glm::ivec2& screenPosition, const glm::vec3& color)
{
	S2DMessage newMessage;
	newMessage.m_Message = message;
	newMessage.m_ScreenPosition = screenPosition;
	newMessage.m_Color = color;

	m_2DMessages.push_back(newMessage);
}

void RenderDataManager::Add2DMessage(const std::string& message, const glm::ivec2& screenPosition)
{
	Add2DMessage(message, screenPosition, glm::vec3(0.5, 0.8f, 0.2f));
}


void RenderDataManager::AddMouseMessage(const std::string& message)
{
	glm::ivec2 mousePos = glm::ivec2(g_InputHandler->GetMousePosition());

	int messageHeight = (int) (g_EngineSettings->GetMessagesPixelsOffset() * 1.1f * m_NumMouseMessages);
	mousePos.y = 990 - mousePos.y - messageHeight;
	Add2DMessage(message, mousePos);

	m_NumMouseMessages++;
}

void RenderDataManager::Update()
{
	auto messageIterator = m_TimedMessages.begin();
	while (messageIterator != m_TimedMessages.end())
	{
		TimedMessage& message = *messageIterator;

		message.m_Timer -= 1;

		if (message.m_Timer == 0)
		{
			messageIterator = m_TimedMessages.erase(messageIterator);
		}
		else
		{
			messageIterator++;
		}
	}

	m_2DMessages.clear();
	m_EntityAABBJobs.clear();
	m_NumMouseMessages = 0;
}

void RenderDataManager::GatherVisibleEntities(const std::vector<Entity*>& entities, Camera* camera)
{
	for (Entity* e : entities)
	{
		bool visible = true;

		if (m_CullingOptions->GetCullEntities() /*&& e == g_Editor->GetPickingTool()->GetPickedEntity()*/)
		{
			visible = IsAABBVisible(e->GetComponent<BoundingBoxComponent>()->GetBoundingBox().GetVertices(), camera);
		}

		if (!visible)
		{
			continue;
		}

		m_VisibleEntities.push_back(e);

		if (m_VisibleEntities.size() == m_MaxVisibleEntities)
		{
			AddTimedMessage(StringUtils::Format("Hit limit of %d entities visible", m_MaxVisibleEntities));
			break;
		}
	}
}

enum LeftRightTestResult
{
	Unknown = 0,
	One = 1,
	Both = 2
};

glm::vec4 GetClipPosFromWorldPosition(const glm::vec4& worldPosition, Camera* camera)
{
	const glm::mat4& vpMatrix = camera->GetViewProjectionMatrix();

	glm::vec4 clipPos = glm::mul(vpMatrix, worldPosition);
	clipPos /= clipPos.w;
	return clipPos;
}
glm::ivec2 GetNDCFromClipPos(const glm::vec4& clipPos, const glm::vec2& windowSize)
{
	return glm::ivec2((0.5f * (glm::vec2(clipPos) + glm::vec2(1, 1))) * windowSize);
}

glm::ivec2 GetNDCFromWorldPosition(const glm::vec4& worldPosition, Camera* camera, const glm::vec2& windowSize)
{
	return GetNDCFromClipPos(GetClipPosFromWorldPosition(worldPosition, camera), windowSize);
}

bool RenderDataManager::IsAABBVisible(const std::vector<glm::vec3>& vertices, Camera* camera)
{
	if (m_CullingOptions->GetUseDotProduct())
	{
		bool allDotsBad = true;
		for (int i = 0; i < 8; ++i)
		{
			const glm::vec3& worldPosition = vertices[i];

			glm::vec3 vertexToCam = worldPosition - camera->GetPosition();

			float dotProduct = glm::dot(glm::normalize(vertexToCam), camera->GetFrontVector());

			bool behind = dotProduct > 0;

			if (m_CullingOptions->GetDebugDotProduct() && vertexToCam.length() < m_CullingOptions->GetDebugDotProductMaxDistance())
			{
				const glm::vec2& windowSize = glm::vec2(Renderer::GetInstance().GetWindowSize());

				glm::ivec2 ndc = GetNDCFromWorldPosition(glm::vec4(worldPosition, 1.f), camera, windowSize);

				int textWidth = 100;
				int edgeOffset = ndc.x + textWidth - windowSize.x;
				ndc.x -= std::min(textWidth, std::max(edgeOffset, 0));

				glm::vec3 msgColor = behind ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0);
				std::stringstream ss;
				ss << "V" << i << " : " << dotProduct;
				Add2DMessage(ss.str(), glm::ivec2(ndc), msgColor);
			}

			if (!behind)
			{
				allDotsBad = false;

				//if (!m_CullingOptions->GetDebugDotProduct())
				//{
				//	return true;
				//}

				break;
			}
		}

		if (allDotsBad)
		{
			return false;
		}
	}

	if (m_CullingOptions->GetUseAABBClipPos())
	{

		const glm::mat4& vpMatrix = camera->GetViewProjectionMatrix();
		bool oneVertexVisible = false;

		bool over_lrtb[4] = { false, false, false, false };
		bool under_lrtb[4] = { false, false, false, false };

		for (int i = 0; i < 8; ++i)
		{
			bool visible = false;

			glm::vec4 worldPosition = glm::vec4(vertices[i], 1.f);

			glm::vec4 clipPos = GetClipPosFromWorldPosition(worldPosition, camera);

			glm::vec4 absClip = glm::abs(clipPos);

			bool inX = absClip.x <= 1.f;
			bool inY = absClip.y <= 1.f;

			if (inX && inY)
			{
				visible = true;

				if (!m_CullingOptions->GetDebugAABBClippPos())
				{
					return true;
				}
			}



			if (m_CullingOptions->GetDebugAABBClippPos())
			{
				const glm::vec2& windowSize = glm::vec2(Renderer::GetInstance().GetWindowSize());

				glm::ivec2 ndc = GetNDCFromClipPos(clipPos, windowSize);

				int textWidth = 100;
				int edgeOffset = ndc.x + textWidth - windowSize.x;
				ndc.x -= std::min(textWidth, std::max(edgeOffset, 0));

				glm::vec3 msgColor = visible ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0);
				std::stringstream ss;
				ss << "V" << i << " : " << ndc.x << " " << ndc.y;
				Add2DMessage(ss.str(), glm::ivec2(ndc), msgColor);
			}

			oneVertexVisible = oneVertexVisible || visible;

			over_lrtb[0] |= clipPos.x > -1;
			under_lrtb[0] |= clipPos.x < -1;
			over_lrtb[1] |= clipPos.x > 1;
			under_lrtb[1] |= clipPos.x < 1;
			over_lrtb[2] |= clipPos.y > -1;
			under_lrtb[2] |= clipPos.y < -1;
			over_lrtb[3] |= clipPos.y > 1;
			under_lrtb[3] |= clipPos.y < 1;
		}

		static bool uselrtb = false;

		if (uselrtb)
		{
			for (int i = 0; i < 4; ++i)
			{
				if (over_lrtb[i] && under_lrtb[i])
				{
					return true;
				}
			}
		}


		bool result = oneVertexVisible;

		return result;
	}

	return true;
}

bool RenderDataManager::IsAABBVisible(const AABB& aabb, Camera* camera)
{
	const std::vector<glm::vec3>& vertices = aabb.GetVertices();

	return IsAABBVisible(vertices, camera);
}

bool RenderDataManager::IsSectorVisible(Sector* sector, Camera* camera)
{
	const AABB& aabb = ((BoundingBoxComponent*)sector->GetTerrainPatchEntity()->GetComponent<BoundingBoxComponent>())->GetBoundingBox();

	bool visible = IsAABBVisible(aabb, camera);;

	return visible;
}


void RenderDataManager::GatherVisibleEntities(World* world, Camera* camera)
{
	lxAssert(world);
	lxAssert(camera);

	m_VisibleEntities.clear();
	m_VisibleSectors.clear();

	GatherVisibleEntities(world->GetEntities(), camera);

	for (Sector* sector : world->GetSectors())
	{
		bool sectorIsVisible = true;
		
		if (m_CullingOptions->GetCullSectors())
		{
			sectorIsVisible = IsSectorVisible(sector, camera);
		}

		if (!sectorIsVisible)
		{
			continue;
		}
		
		m_VisibleSectors.push_back(sector);

		GatherVisibleEntities(sector->GetGraphicalData()->GetRoadEntities(), camera);
		GatherVisibleEntities(sector->GetGraphicalData()->GetStaticEntities(), camera);

	}

	m_RenderingStats->SetNumVisibleEntities(m_VisibleEntities.size());
	m_RenderingStats->SetNumVisibleSectors(m_VisibleSectors.size());
}
