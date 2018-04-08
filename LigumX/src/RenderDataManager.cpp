#include "stdafx.h"

#include "RenderDataManager.h"
#include "LigumX.h"
#include "Sector.h"
#include "Renderer.h"
#include "Way.h"
#include "Node.h"
#include "World.h"
#include "SectorData.h"
#include "SectorGraphicalData.h"
#include "Heightfield.h"
#include "Logging.h"
#include "Mesh.h"
#include "Model.h"
#include "Material.h"
#include "Editor.h"
#include "EngineSettings.h"
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

void RenderDataManager::CreateWaysLines(Sector* sector)
{
	SectorGraphicalData* gfxData = sector->GetGraphicalData();

	std::vector<glm::vec3> nodePositions;

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

		int fillFlags = OSMElement::BUILDING_UNMARKED | 
						OSMElement::BUILDING_SCHOOL | 
						OSMElement::LEISURE_PARK | 
						OSMElement::NATURAL_WOOD | 
						OSMElement::NATURAL_WATER | 
						OSMElement::LANDUSE;
		if ((way->eType & fillFlags) != 0)
		{
			Building building(way);
			bool success = building.GenerateModel();

			if (success)
			{
				renderer.AddToDebugModels(building.m_Model);

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


	Mesh* nodeMesh = new Mesh(nodePositions, GL::PrimitiveMode::Points, true);

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
	m_TimedMessages.push_back(TimedMessage(message, g_EngineSettings->GetMessagesDefaultFrameCount()));
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
}