#include "stdafx.h"

#include "BoundingBoxComponent.h"
#include "RenderDataManager.h"
#include "InputHandler.h"
#include "LigumX.h"
#include "StringUtils.h"
#include "Editor.h"
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

using namespace std;
using namespace glm;


FlatWaysMesh::FlatWaysMesh(const std::vector<glm::vec3>& vertices, const std::vector<WayData>& dataBuffer, GL::PrimitiveMode primitiveMode, bool usePointRendering)
{
	m_buffers.vertexPositions = vertices;

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
	LigumX::GetInstance().m_Renderer->createGLBuffer(GL_ARRAY_BUFFER, GetGPUBuffers().glidPositions, m_buffers.vertexPositions);

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
	m_buffers.vertexPositions = vertices;

	m_ColorBuffer = dataBuffer;
	m_PrimitiveMode = primitiveMode;
	SetPointRendering(usePointRendering);

	SetUsesIndexBuffer(false);

	CreateBuffers();
}

void ColoredPointsMesh::CreateBuffers()
{
	// TODO: I'm not quite convinced this belongs here. or does it?
	LigumX::GetInstance().m_Renderer->createGLBuffer(GL_ARRAY_BUFFER, GetGPUBuffers().glidPositions, m_buffers.vertexPositions);
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
						subdivisionPoint = pos + (float)(j + 1) * direction * length / factor;

						subdivisionPoint.z = world->SampleHeight(subdivisionPoint);

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

void RenderDataManager::fillBuffers(Sector* sector)
{
    LigumX& game = LigumX::GetInstance();
    Renderer& renderer = Renderer::GetInstance();

    bool first;
    bool second;
    vec3 white = vec3(1.0f,1.0f,1.0f);
    unsigned int nbSuccessLoops = 0;
    unsigned int nbFailedLoops = 0;
    int LiftableWalls = OSMElement::BUILDING_UNMARKED;
    unsigned int nbBuildings = 0;

    GLint firstVertexForThisRoad = 0;

    for ( auto it = sector->m_Data->ways.begin(); it != sector->m_Data->ways.end(); ++it )
    {

        first = true;
        second = false;
        GLsizei nbVertexForThisRoad = 0;
        vec3 pt, oldpt, oldoldpt;
        vec3 point, oldPoint, oldOldPoint;
        vec3 color = vec3(glm::linearRand(0.5f, 1.0f),
                          glm::linearRand(0.5f, 1.0f),
                          glm::linearRand(0.5f, 1.0f));
        Way* way = it->second;
        try
        {
            color = renderer.typeColorMap[way->eType];
        }
        catch(...)
        {
            continue;
        }

        for (auto nodeIt = way->GetNodes().begin() ; nodeIt != way->GetNodes().end(); ++nodeIt)
        {
            
            Node* node = *nodeIt;
            point = vec3(node->longitude, node->latitude, node->elevation);
            pt = vec3(node->longitude, node->latitude, node->elevation);
            nodesPositions.push_back(pt);
            if (!first)
            {

                //if (way->eType == OSMElement::GRID_LINE)
                //{
                //    continue; 
                //}

                //else
                {
                    waysNodesPositions.push_back(oldpt);
                    waysNodesColors.push_back(color);
                    waysNodesPositions.push_back(pt);
                    waysNodesColors.push_back(color);

                    waysNodesPositionsMap[way->eType].push_back(oldpt);
                    waysNodesPositionsMap[way->eType].push_back(pt);

                }

                // create roads subgroup
                if (way->hasTag("highway")) 
                {
                    if(second) 
                    {
                        second = false;
                        // first point for line adjacency, fake straight line
                        roadsPositions.push_back(oldPoint - (point-oldPoint));
                        roadsPositions.push_back(oldPoint);
                        nbVertexForThisRoad += 2;
                    }
                    roadsPositions.push_back(point);
                    ++nbVertexForThisRoad;
                }
            }
            else
            {
                first = false;
                //if (way->hasTagAndValue("highway", "residential")) {
                if (way->hasTag("highway")) 
                {
                    second = true;
//                    roadsPositions->push_back(point); // first point for line adjacency
//                    ++nbVertexForThisRoad;
                }
            }

            oldOldPoint = oldPoint;
            oldPoint = point;

            oldoldpt = oldpt;
            oldpt = pt;
        }

        // push last road node for line adjacency, fake straight line
        //if (way->hasTagAndValue("highway", "residential")) {
        if (way->hasTag("highway")) 
        {
            roadsPositions.push_back(point + (point-oldOldPoint));
            ++nbVertexForThisRoad;
            ++nbRoads;

            renderer.firstVertexForEachRoad.push_back(firstVertexForThisRoad);
            renderer.nbVerticesForEachRoad.push_back(nbVertexForThisRoad);

            // set first vertex index for next road
            firstVertexForThisRoad += nbVertexForThisRoad;
        }

        // triangulate building loops
        // PRINTELEMENTPTR(way);
        Node first(*(way->GetNodes().front()));
        Node last(*(way->GetNodes().back()));
        if((way->eType==OSMElement::BUILDING_UNMARKED||way->eType == OSMElement::LEISURE_PARK)&&way->GetNodes().size() >= 3 && first == last)
        {
            //Building* building = new Building(way);
            // building->GenerateModel();
        }
    }

    renderer.nbWaysVertices = (int) waysNodesPositions.size();
    renderer.nbBuildingTriangles = (int) buildingTrianglePositions.size();
    renderer.nbBuildingLines = (int) buildingSides.size();
    renderer.nbRoads = nbRoads;

    // Renderer::createGLBuffer(renderer.glidNodesPositions, nodesPositions);
    // Renderer::createGLBuffer(renderer.glidBufferRoadsPositions, roadsPositions);
    // Renderer::createGLBuffer(renderer.glidBufferBuildingTriangleVertices, buildingTrianglePositions);
    // Renderer::createGLBuffer(renderer.glidBufferBuildingLines, buildingSides);
    // Renderer::createGLBuffer(renderer.glidBufferBuildingLoopLengths, buildingLoopLengths);

    //TODO: Material should still haeve a nice constructor.
    Model* waysModel = new Model();
    for (auto it = waysNodesPositionsMap.begin(); it != waysNodesPositionsMap.end(); ++it)
    {
        OSMElement::ElementType index = it->first;

        Mesh* mesh = new Mesh(it->second, GL::PrimitiveMode::Lines);

        waysModel->addMesh( mesh, new Material(renderer.pPipelineLines, renderer.typeColorMap[index]) );
    }

    waysModel->SetName("Ways");
    renderer.AddToDebugModels(waysModel);
    
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

std::vector<AABBJob>& RenderDataManager::GetAABBJobs()
{
	return m_AABBJobs;
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

void RenderDataManager::Add2DMessage(const std::string& message, const glm::ivec2& screenPosition)
{
	S2DMessage newMessage;
	newMessage.m_Message = message;
	newMessage.m_ScreenPosition = screenPosition;

	m_2DMessages.push_back(newMessage);
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
	m_NumMouseMessages = 0;
}

void RenderDataManager::GatherVisibleEntities(const std::vector<Entity*>& entities, Camera* camera)
{
	for (Entity* e : entities)
	{
		m_VisibleEntities.push_back(e);
		m_NumVisibleEntities++;

		if (m_NumVisibleEntities == m_MaxVisibleEntities)
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
bool RenderDataManager::IsAABBVisible(const AABB& aabb, Camera* camera)
{
	const glm::mat4& vpMatrix = camera->GetViewProjectionMatrix();

	glm::ivec4 plane = glm::ivec4(-9999, -9999, -9999, -9999);

	bool allDotPositive = true;
	for (int i = 0; i < 8; ++i)
	{
		glm::vec3 worldPosition = aabb.GetVertices()[i];
		worldPosition.z = camera->GetPosition().z;

		glm::vec3 vertexToCam = worldPosition - camera->GetPosition();

		float dotProduct = glm::dot(glm::normalize(vertexToCam), camera->GetFrontVector());

		if (dotProduct < 0.f)
		{
			allDotPositive = false;
			break;
		}
	}

	if (allDotPositive)
	{
		return false;
	}


	bool allUnder[4] = { true, true, true, true };
	bool allOver[4] = { true, true, true, true };
	std::vector<glm::vec2> ndcPositions;
	for (int i = 0; i < 8; ++i)
	{
		bool isVertexVisible = false;

		glm::vec4 worldPosition = glm::vec4(aabb.GetVertices()[i], 1.f);

		glm::vec4 clipPos = glm::mul(vpMatrix, worldPosition);

		glm::vec4 ogClipPos = clipPos;

		//if (clipPos.w < 0)
		//{
		//	return false;
		//}

		//clipPos.y *= -1;
		clipPos /= clipPos.w;

		vec2 ndc = 0.5f * (glm::vec2(clipPos) + glm::vec2(1, 1));
		ndcPositions.push_back(ndc);

		float refNDC[4] = { 0, 1, 0, 1 };
		for (int p = 0; p < 4; ++p)
		{
			int n = p / 2;
			float side = ndc[n];
			float correctedCoord = side - refNDC[p];

			if (correctedCoord > 10.f)
			{
				return false;
			}

			allUnder[p] &= correctedCoord < 0;
			allOver[p] &= correctedCoord > 0;



			//int n = ndcIndex[p];

			//float side = ndc[n];

			//float correctedCoord = side - refNDC[p];

			//if (plane[p] != -9999)
			//{
			//	if (correctedCoord * plane[p] < 0.f)
			//	{

			//		return true;
			//	}
			//	else
			//	{
			//		plane[p] += sign(correctedCoord);
			//	}
			//}
			//else
			//{
			//	plane[p] = sign(correctedCoord);
			//}

			//if (p == 1) break;

			//break;
		}



		//if (allUnder[2] || allOver[3])
		//{
		//	return false;
		//}


	}

	if (allUnder[0] || allOver[1] || allUnder[2] || allOver[3])
	{
		return false;
	}

	//if (plane[0] == 8 && plane[1] == -8 /*&& plane[2] == 8 && plane[3] == -8*/)
	//{
	//	return true;
	//}

	for (int i = 0; i < 8; ++i)
	{

	}

	return true;
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
	m_NumVisibleEntities = 0;

	m_VisibleSectors.clear();
	m_NumVisibleSectors = 0;

	GatherVisibleEntities(world->GetEntities(), camera);

	for (Sector* sector : world->GetSectors())
	{
		bool sectorIsVisible = true;
		
		if (g_EngineSettings->GetCullSectors())
		{
			sectorIsVisible = IsSectorVisible(sector, camera);
		}

		if (!sectorIsVisible)
		{
			continue;
		}
		
		m_VisibleSectors.push_back(sector);
		m_NumVisibleSectors++;
		if (m_NumVisibleSectors == m_MaxVisibleSectors)
		{
			AddTimedMessage(StringUtils::Format("Hit limit of %d sectors visible", m_MaxVisibleSectors));
			break;
		}

		GatherVisibleEntities(sector->GetGraphicalData()->GetRoadEntities(), camera);
		GatherVisibleEntities(sector->GetGraphicalData()->GetStaticEntities(), camera);

	}

	AddTimedMessage(StringUtils::Format("Sectors visible: %d", m_NumVisibleSectors), 2);
}