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
#include "DefaultMeshes.h"

#pragma region  CLASS_SOURCE World
#include "World.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectIdManager.h"
const ClassPropertyData World::g_Properties[] = 
{
{ "ObjectID", offsetof(World, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "Name", offsetof(World, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
{ "SunLight", offsetof(World, m_SunLight), 0, LXType_SunLight, true, LXType_None, 0, 0, 0, }, 
{ "Entities", offsetof(World, m_Entities), 0, LXType_stdvector, false, LXType_Entity, 0, 0, 0, }, 
};
void World::Serialize(bool writing)
{
	g_Serializer->SerializeObject(this, writing); 
}

#pragma endregion  CLASS_SOURCE World
using namespace std;
using namespace glm;


World::World()
{
	m_ObjectID = g_ObjectIDManager->GetObjectID();

}

void World::InitTestWorld()
{


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
		cubeModel->SetFilenameIsIDCallback(true);
		cubeModel->SetName("CubeLightModel");
		pointLightEntity->SetModel(cubeModel);

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
		sphereModel->SetFilenameIsIDCallback(true);

		sphereEntity->SetModel(sphereModel);

		m_Entities.push_back(sphereEntity);
	}

}

World::World(float sectorSize)
{
	m_ObjectID = g_ObjectIDManager->GetObjectID();

	m_sectorManager = new SectorManager(sectorSize);
	m_sectorSize = sectorSize;
	m_invSectorSize = 1.f / sectorSize;

	//m_SunLight = new SunLight();
	//m_SunLight->SetObjectID(3796);
	//m_SunLight->Serialize(false);

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
