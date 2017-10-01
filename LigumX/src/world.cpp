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
#include "Texture.h"
#include "Mesh.h"

using namespace std;
using namespace glm;


World::World()
{
}

World::World(float sectorSize)
{
	m_sectorManager = new SectorManager(sectorSize);
	m_sectorSize = sectorSize;
	m_invSectorSize = 1.f / sectorSize;

	Model* testModel = new Model("C:/Users/Joel/Documents/LigumX/LigumX/data/models/nanosuit/nanosuit2.obj");
	testModel->SetName("NanosuitModel");
	//Material* testMaterial = new Material(new ProgramPipeline("Basic"), glm::vec3(1,0,0));
	//testModel->m_materialList.push_back(testMaterial);

	Entity* testEntity = new Entity();
	testEntity->setPosition(glm::vec3(0, 0, 1));
	testEntity->SetModel(testModel);

	glm::mat4x4 toWorld = glm::translate(glm::mat4(1.0), glm::vec3(0, 20, 1));
	toWorld = glm::rotate(toWorld, 90.f, glm::vec3(1, 0, 0));
	testEntity->m_ModelToWorldMatrix = toWorld;
	m_Entities.push_back(testEntity);
	testEntity->SetName("Nanosuit");
	
	Entity* testEntity2 = new Entity();

	// todo : properly handle setposition with modelmatrix
	testEntity2->setPosition(glm::vec3(0, 2, 0));

	//glm::mat4x4 toWorld2 = glm::translate(glm::mat4(1.0), glm::vec3(-100, 100, 1));
	//toWorld2 = glm::scale(toWorld2, glm::vec3(200, 200, 1));
	//toWorld2 = glm::rotate(toWorld2, 180.f, glm::vec3(1, 0, 0));

	glm::mat4x4 toWorld2 = glm::translate(glm::mat4(1.0), glm::vec3(-100, -100, 1));
	toWorld2 = glm::scale(toWorld2, glm::vec3(200, 200, 1));
	//toWorld2 = glm::rotate(toWorld2, 180.f, glm::vec3(1, 0, 0));
	//toWorld2 = glm::rotate(toWorld2, 90.f, glm::vec3(0, 1, 0));

	Material* material = new Material(testModel->m_materialList[4]->GetProgramPipeline(), glm::vec3(1, 0, 0));
	material->SetShininess(1.0f);
	material->SetDiffuseTexture(new Texture("C:/Users/Joel/Documents/LigumX/LigumX/data/textures/grass.png"));
	testModel = new Model(g_DefaultMeshes->DefaultQuadMesh, material);
	testModel->SetName("TerrainModel");
	//testModel = new Model("Nanosuit", "C:/Users/Joel/Documents/LigumX/LigumX/data/models/sphere/sphere.obj");
	testEntity2->SetModel(testModel);
	testEntity2->m_ModelToWorldMatrix = toWorld2;
	testEntity2->SetName("Terrain");
	m_Entities.push_back(testEntity2);

	m_SunLight = new SunLight();
	m_SunLight->SetTime(100.f);
	m_SunLight->SetOrientation(25.f);
	m_SunLight->SetSpeed(0.f);
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