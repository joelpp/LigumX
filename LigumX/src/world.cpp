#include "stdafx.h"

#include "world.h"
#include "sector.h"
#include "Way.h"
#include "Node.h"
#include "Math.h"
#include "SectorData.h"
#include "vec2i.h"
#include "Entity.h"
#include "SectorManager.h"
#include "Material.h"
#include "Model.h"
#include "SunLight.h"
#include "GL.h"
#include "Texture.h"
#include "Mesh.h"

#pragma region  CLASS_SOURCE World
#include "World.h"
#include "serializer.h"
#include <cstddef>
const ClassPropertyData World::g_Properties[] = 
{
{ "ObjectID", offsetof(World, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "SunLight", offsetof(World, m_SunLight), 0, LXType_SunLight, true, LXType_None, 0, 0, 0, }, 
};
void World::Serialize(bool writing)
{
	std::string basePath = "C:\\Users\\Joel\\Documents\\LigumX\\LigumX\\data\\objects\\";
	std::string fileName = "World_" + std::to_string(m_ObjectID) + ".LXobj";

	int fileMask = writing ? std::ios::out : std::ios::in;
	std::fstream objectStream(basePath + fileName, fileMask);

	if (objectStream.is_open())
	{
		Serializer::SerializeObject(this, objectStream, writing);
	}
}

#pragma endregion  CLASS_SOURCE World
using namespace std;
using namespace glm;


World::World()
{
	m_ObjectID = rand();

}

void World::InitTestWorld()
{
	Model* nanosuitModel;
	{
		Entity* testEntity = new Entity();
		testEntity->SetName("Nanosuit");
		testEntity->SetPosition(glm::vec3(0, 20, 1));
		testEntity->SetRotationAngle(90.f);

		nanosuitModel = new Model("C:/Users/Joel/Documents/LigumX/LigumX/data/models/nanosuit/nanosuit2.obj");
		nanosuitModel->SetName("NanosuitModel");
		testEntity->SetModel(nanosuitModel);

		m_Entities.push_back(testEntity);
	}

	{
		Entity* testEntity = new Entity();
		testEntity->SetName("Terrain");
		testEntity->SetPosition(glm::vec3(-100, -100, -1));
		testEntity->SetScale(glm::vec3(200, 200, 1));

		Material* material = new Material(nanosuitModel->GetMaterials()[4]->GetProgramPipeline(), glm::vec3(1, 0, 0));
		material->SetShininess(1.0f);

		Texture* diffuseTexture = new Texture("C:/Users/Joel/Documents/LigumX/LigumX/data/textures/grass.png");
		diffuseTexture->Initialize();
		material->SetDiffuseTexture(diffuseTexture);

		Model* terrainQuadModel = new Model(g_DefaultMeshes->DefaultQuadMesh, material);
		terrainQuadModel->SetName("TerrainModel");
		testEntity->SetModel(terrainQuadModel);

		m_Entities.push_back(testEntity);
	}

	// light 0
	{
		Entity* pointLightEntity = new Entity();
		pointLightEntity->SetName("PointLight");
		pointLightEntity->SetPosition(glm::vec3(-7.f, 11.f, 13.8f));
		pointLightEntity->SetScale(glm::vec3(2,2,2));
		pointLightEntity->SetIsLight(true);

		Material* material = new Material();
		material->SetShininess(1.0f);
		material->SetDiffuseColor(glm::vec3(0, 1, 0));
		material->SetUnlit(true);
		material->SetEmissiveFactor(1.0f);

		Model* cubeModel = new Model(g_DefaultMeshes->DefaultCubeMesh, material);
		cubeModel->SetName("CubeLightModel");
		pointLightEntity->SetModel(cubeModel);

		m_Entities.push_back(pointLightEntity);
	}

	// light 1
	//if (false)
	{
		Entity* pointLightEntity = new Entity();
		pointLightEntity->SetName("PointLight");
		pointLightEntity->SetPosition(glm::vec3(7.f, 11.f, 13.8f));
		pointLightEntity->SetScale(glm::vec3(2, 2, 2));
		pointLightEntity->SetIsLight(true);

		Material* material = new Material();
		material->SetShininess(1.0f);
		material->SetDiffuseColor(glm::vec3(1, 0, 0));
		material->SetUnlit(true);
		material->SetEmissiveFactor(1.0f);

		Model* cubeModel = new Model(g_DefaultMeshes->DefaultCubeMesh, material);
		cubeModel->SetName("CubeLightModel");
		pointLightEntity->SetModel(cubeModel);

		m_Entities.push_back(pointLightEntity);
	}

	// light 3
	//if (false)
	{
		Entity* pointLightEntity = new Entity();
		pointLightEntity->SetObjectID(19817);
		pointLightEntity->Serialize(false);

		pointLightEntity->GetModel()->addMesh(g_DefaultMeshes->DefaultCubeMesh);

		m_Entities.push_back(pointLightEntity);
	}

	// sphere
	//if (false)
	{
		Entity* sphereEntity = new Entity();
		sphereEntity->SetName("Sphere");
		sphereEntity->SetPosition(glm::vec3(0.f, 7.f, 10.f));
		sphereEntity->SetScale(glm::vec3(2, 2, 2));

		Material* material = new Material();
		material->SetShininess(1.0f);
		material->SetDiffuseColor(glm::vec3(1,1, 1));

		Model* sphereModel = new Model(g_DefaultMeshes->DefaultSphereMesh, material);
		sphereModel->SetName("SphereModel");
		sphereEntity->SetModel(sphereModel);

		m_Entities.push_back(sphereEntity);
	}

}

World::World(float sectorSize)
{
	m_ObjectID = rand();

	m_sectorManager = new SectorManager(sectorSize);
	m_sectorSize = sectorSize;
	m_invSectorSize = 1.f / sectorSize;

	InitTestWorld();

	m_SunLight = new SunLight();
	m_SunLight->SetObjectID(3796);
	m_SunLight->Serialize(false);

	//m_SunLight->SetTime(100.f);
	//m_SunLight->SetOrientation(25.f);
	//m_SunLight->SetSpeed(0.f);

	//Texture* skyTexture = new Texture("C:/Users/Joel/Documents/LigumX/LigumX/data/textures/skybox/", true);
	//skyTexture->Initialize();
	//m_SunLight->SetSkybox(skyTexture);
	//m_SunLight->SetUseSkybox(true);

}



Coord2 World::Normalized(Coord2 UnNormalized)
{
	return UnNormalized * m_invSectorSize;
}

Coord2 World::sectorPosFromXY(Coord2 longLat)
{
	Coord2 normalized = Normalized(longLat);
	normalized = glm::floor(normalized);

	return normalized * m_sectorSize;
}



Sector* World::GetOrCreateSectorContainingXY(Coord2 longLat)
{

	Sector* sector = m_sectorManager->sectorContaining(longLat);

	return sector;
}


SectorList* World::sectorsAroundPoint(Coord2 point, int ringSize)
{
	return m_sectorManager->sectorsAround(point, ringSize, false);
}

SectorList* World::updateSectorsAroundPoint(Coord2 point, int ringSize)
{
	SectorList* sectors = m_sectorManager->sectorsAround(point, ringSize, true);

	//for(int i = 0; i < sectors->size(); ++i)
	//{
	//
	//    Sector* sector = sectors->at(i);
	//    if (sector->m_initializationLevel < Sector::FullyInitialized)
	//    {
	//        if (sector->m_initializationLevel == Sector::ContourLoaded)
	//        {
	//            renderData->initializeSector(sector);
	//        }

	//        if (sector->m_initializationLevel == Sector::DataLoaded)
	//        {
	//            renderData->fillBuffers(sector);
	//        }

	//        if (sector->m_initializationLevel == Sector::HeightfieldGenerated)
	//        {
	//            //PRINT("heightfield geenerated, adding to terrain buffer")
	//             renderData->addToTerrainBuffer(sector);
	//        }
	//    }


	//}
	return sectors;
}

SectorList* World::GetAllSectors()
{
	return 0;// m_sectorManager->GetAllSectors();
}

std::vector<Way*> World::findNClosestWays(int n, glm::vec2 xy, 
									  std::vector<Way*> querySet, int filter, 
									  std::vector<double> &distances, 
									  std::vector<glm::vec2> &_directions, 
									  std::vector<std::pair<Node*, Node*>> &_nodePairs)
{
    Way* closest = NULL;
    std::vector<Way*> closests;
    std::vector<double> bestDists;
    std::vector<vec2> directions;
    std::pair<Node*, Node*> a;
    vector<std::pair<Node*, Node*>> nodePairs;
    for (int i = 0; i < n; i++){
        bestDists.push_back(99999);
        closests.push_back(NULL);
        directions.push_back(vec2(0,0));
        nodePairs.push_back(a);
    }

    //Look at all the ways
    for ( auto it = querySet.begin(); it != querySet.end(); ++it ){
         Way* way = *it;
//        if (!way->selectable) continue;

        // This way's type not yet implemented :( get to it!
        if (way->eType == OSMElement::NOT_IMPLEMENTED) continue;

        // A specific filter has been chosen. Check if this way passes it!
        if (filter != OSMElement::ANY_TYPE)
            if ((way->eType & filter) == 0) continue;

        Node* firstNode = NULL;
        Node* secondNode = NULL;
        int counter = 1;
        bool first = true;
        vec2 p1, p2;
        double bestDist = 99999;
        vec2 direction;
        std::pair<Node*, Node*> aPair;

        //Look at all this way's nodes
        for (auto nodeIt = way->nodes.begin() ; nodeIt != way->nodes.end(); ++nodeIt){

            // Keep first node and continue
            if (first){
                firstNode = *nodeIt;
                first = false;
                continue;
            }

            // Prepare next pair by putting the old second in the new first
            if (counter == 0) firstNode = secondNode;
            // Look at next pair by looking at new second node
            else{
                secondNode = *nodeIt;
                p1 = vec2(firstNode->longitude, firstNode->latitude);
                p2 = vec2(secondNode->longitude, secondNode->latitude);
                vec2 dir;
                double dist = Math::pointLineSegmentDistance(xy, p2, p1, dir);

                if (dist < bestDist){
                    closest = way;
                    bestDist = dist;
                    direction = dir;
                    aPair = std::pair<Node*, Node*>(firstNode, secondNode);
                }
            }

            //flip-flop
            counter = (counter + 1) % 2;
        }
        //Now you're done looking at all the nodes.
        //Iterate over all current closest list, from closest to farthest
        for (int i = 0; i < n; i++){

            // Otherwise, we found a smaller distance (the way may have been seen or not)
            if (bestDist <= bestDists[i]){


                // Move everything behind this element farther by 1
                for (int j = n-1; j > i; j--){
                    bestDists[j] = bestDists[j-1];
                    closests[j] = closests[j-1];
                    directions[j] = directions[j-1];
                    nodePairs[j] = nodePairs[j-1];
                }

                // Keep the way and the distance.
                bestDists[i] = bestDist;
                closests[i] = way;
                directions[i] = direction;
                nodePairs[i] = aPair;
                break; //get out! you don't want to keep going in the array.

            }

        }

    }
    _directions = directions;
    distances = bestDists;
    _nodePairs = nodePairs;
    return closests;
}

std::vector<Way*> World::getAllContourLines(SectorList* sectors)
{
	std::vector<Way*> contours;
	for (auto sectorIt = sectors->begin(); sectorIt != sectors->end(); ++sectorIt)
	{
		Sector* sector = *sectorIt;
		for (auto wayIt = sector->m_data->ways.begin(); wayIt != sector->m_data->ways.end(); ++wayIt)
		{
			contours.push_back(wayIt->second);

		}
	}

	return contours;
}

void World::Update()
{
	for (Entity* entity : m_Entities)
	{
		entity->Update(0);
	}
}
