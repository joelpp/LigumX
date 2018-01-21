#include "stdafx.h"

#include "RenderDataManager.h"
#include "LigumX.h"
#include "Sector.h"
#include "Renderer.h"
#include "Way.h"
#include "Node.h"
#include "Triangle.h"
#include "World.h"
#include "SectorData.h"
#include "Heightfield.h"
#include "Logging.h"
#include "Mesh.h"
#include "Model.h"
#include "Material.h"
#include "Editor.h"
#include "Building.h"
#include "glm/gtx/transform.hpp"

RenderDataManager* g_RenderDataManager;

using namespace std;
using namespace glm;

int findSetBit(int number){
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

RenderDataManager::RenderDataManager(){
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

    //Model* model = new Model("C:/Users/Joel/Documents/LigumX/LigumX/data/models/sphere.obj");
    //testMesh = model->m_meshes[0];

    //glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0), glm::vec3(-0.6, 0.55, 0));
    //glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0),  0.f, glm::vec3(0, 0, 1));
    //glm::mat4 scaleMatrix = glm::scale(0.01f, 0.01f, 0.01f);
    //model->m_modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

    //testMesh->createBuffers();
    //testMesh->m_usesIndexBuffer = false;
    //testMesh->m_renderingMode = GL_TRIANGLES;

    //model->m_materialList.push_back(  new Material(renderer.pPipelineBasicUV, glm::vec3(1,1,1))  ) ;

    //model->name = "Sphere 1";
    //renderer.m_debugModels.push_back( model );
    PRINTSTRING("GERREREEGRG");
}

void RenderDataManager::addToTerrainBuffer(Sector* newSector)
{
    LigumX& game = LigumX::GetInstance();
    Renderer& renderer = Renderer::GetInstance();
    int counter = 0;

    Heightfield* heightField = newSector->m_heightfield;

//    if (heightField->m_mesh)
//    {
//        Model* hfModel = new Model();
//        
//        //  hfModel->addMesh( Heightfield::hfBaseMesh, new Material(renderer.pPipelineBasicUV, glm::vec3(1,1,1)) );
//        hfModel->addMesh( heightField->m_mesh, new Material(renderer.pPipelineBasicUV, glm::vec3(1,1,1)) );
//        hfModel->name = "heightfield";
//        
//        renderer.m_debugModels.push_back( hfModel );
//    }
    
    TerrainRenderingJob job;
    job.start = heightField->startPoint;
    job.scale = heightField->sideLength;
    job.buffer = heightField->buffer;
    terrainRenderingJobs.push_back(job);
}

Mesh* RenderDataManager::terrainMesh()
{
    return Heightfield::hfBaseMesh;
}


void RenderDataManager::AddDebugModel(std::vector<glm::vec3>& line, glm::vec3 color)
{
	Renderer& renderer = Renderer::GetInstance();

	Mesh* linesMesh = new Mesh(line, GL_LINES);

	Model* linesModel = new Model();
	linesModel->addMesh(linesMesh, new Material(renderer.pPipelineLines, color));
	linesModel->SetName("Sector_lines_");

	renderer.m_DebugModels.push_back(linesModel);
}

void AddPoint(std::vector<glm::vec3>& points, glm::vec3 point)
{
	if (points.size() > 1)
	{
		points.push_back(points.back());
	}

	points.push_back(point);
}

void RenderDataManager::CreateWaysLines(Sector* sector)
{
	float latMid = sector->GetPosition().x;

	glm::vec2 degreesToMeters;
	degreesToMeters.x = 111132.954f * cos(latMid);
	degreesToMeters.y = 111132.954f - 559.822f * cos(2.f * latMid) + 1.175f * cos(4.f * latMid);

	for (auto it = sector->m_Data->ways.begin(); it != sector->m_Data->ways.end(); ++it)
	{
		std::vector<glm::vec3> line;
		Way* way = it->second;

		for (auto nodeIt = way->GetNodes().begin(); nodeIt != way->GetNodes().end(); ++nodeIt)
		{
			Node* node = *nodeIt;

			glm::vec3 pos = node->getLatLongEle();

			// offset between 0 and extent
			pos -= glm::vec3(sector->GetPosition(), 0.f);

			// scale between 0 and 1
			pos /= glm::vec3(sector->m_LifeSize, 1.f);

			pos *= 200.f;

			World* world = LigumX::GetInstance().getWorld();
			
			const glm::ivec2& baseOffset = sector->GetOffsetIndex();

			pos += glm::vec3(glm::vec2(baseOffset) * 200.f, 0);

			pos.z = 1.f;

			AddPoint(line, pos);
		}

		Renderer& renderer = Renderer::GetInstance();
		glm::vec3 color = renderer.typeColorMap[way->eType];
		AddDebugModel(line, color);
	}
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

                if (way->eType == OSMElement::GRID_LINE)
                {
                    continue; 
                }

                else
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
            Building* building = new Building(way);
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

        Mesh* mesh = new Mesh(it->second, GL_LINES);

        waysModel->addMesh( mesh, new Material(renderer.pPipelineLines, renderer.typeColorMap[index]) );
    }

    waysModel->SetName("Ways");
    renderer.m_DebugModels.push_back(waysModel);
    
    // Model* nodesModel = new Model();

    // Mesh* nodesMesh = new Mesh(nodesPositions, GL_POINTS, true);

    // nodesModel->name = "Nodes";
    // nodesModel->addMesh( nodesMesh, new Material(renderer.pPipelineNodes, glm::vec3(0,0,0)) );
    // renderer.m_debugModels.push_back(nodesModel);
}


void RenderDataManager::InitializeSector(Sector* sector)
{
	const glm::ivec2& baseOffset = sector->GetOffsetIndex();

	float offset = 200.f;
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
		color = glm::vec3(1, 0, 0);
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

std::vector<AABBJob>& RenderDataManager::GetAABBJobs()
{
	return m_AABBJobs;
}

void RenderDataManager::ClearAABBJobs()
{
	m_AABBJobs.clear();
}