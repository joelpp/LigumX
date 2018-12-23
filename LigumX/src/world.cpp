#include "stdafx.h"

#include "world.h"
#include "sector.h"
#include "EngineSettings.h"
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
#include "DefaultObjects.h"
#include "LXError.h"

#pragma region  CLASS_SOURCE World

#include "World.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
#include "SunLight.h"
const ClassPropertyData World::g_Properties[] = 
{
{ "SunLight", PIDX_SunLight, offsetof(World, m_SunLight), 0, LXType_ObjectPtr, sizeof(SunLight*), LXType_SunLight, true, LXType_None, false, 0, 0, 0, 0,}, 
{ "Entities", PIDX_Entities, offsetof(World, m_Entities), 0, LXType_stdvector, sizeof(std::vector<Entity*>), LXType_stdvector, false, LXType_Entity, true, 0, 0, 0, 0,}, 
{ "DebugEntities", PIDX_DebugEntities, offsetof(World, m_DebugEntities), 0, LXType_stdvector, sizeof(std::vector<Entity*>), LXType_stdvector, false, LXType_Entity, true, PropertyFlags_Transient, 0, 0, 0,}, 
{ "Sectors", PIDX_Sectors, offsetof(World, m_Sectors), 0, LXType_stdvector, sizeof(std::vector<Sector*>), LXType_stdvector, false, LXType_Sector, true, PropertyFlags_Transient, 0, 0, 0,}, 
};
bool World::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	PostSerialization(writing, success);
	return success;
}
bool World::ShowPropertyGrid()
{
	LXIMGUI_SHOW_OBJECTREF("SunLight", m_SunLight, SunLight);
	return true;
}

#pragma endregion  CLASS_SOURCE World
using namespace std;
using namespace glm;


World::World()
{
	SetObjectID(g_ObjectManager->GetNewObjectID());
}



void World::PostSerialization(bool writing, bool success)
{
	if (!writing)
	{
		m_DebugEntities.push_back(g_DefaultObjects->DefaultManipulatorEntity);

		for (Entity* entity : m_Entities)
		{
			entity->UpdateAABB();
		}
	}


}

World::World(float sectorSize)
{
	SetObjectID(g_ObjectManager->GetNewObjectID());

	m_sectorManager = new SectorManager(sectorSize);
	m_sectorSize = sectorSize;
	m_invSectorSize = 1.f / sectorSize;
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

	Sector* sector = nullptr;// m_sectorManager->sectorContaining(longLat);
	lxAssert0();
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
	//            m_RenderDataManager->InitializeSector(sector);
	//        }

	//        if (sector->m_initializationLevel == Sector::DataLoaded)
	//        {
	//            m_RenderDataManager->fillBuffers(sector);
	//        }

	//        if (sector->m_initializationLevel == Sector::HeightfieldGenerated)
	//        {
	//            //PRINT("heightfield geenerated, adding to terrain buffer")
	//             m_RenderDataManager->addToTerrainBuffer(sector);
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

        Node* firstNode = NULL;
        Node* secondNode = NULL;
        int counter = 1;
        bool first = true;
        vec2 p1, p2;
        double bestDist = 99999;
        vec2 direction;
        std::pair<Node*, Node*> aPair;

        //Look at all this way's nodes
        for (auto nodeIt = way->GetNodes().begin() ; nodeIt != way->GetNodes().end(); ++nodeIt){

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
		for (auto wayIt = sector->m_Data->ways.begin(); wayIt != sector->m_Data->ways.end(); ++wayIt)
		{
			contours.push_back(wayIt->second);

		}
	}

	return contours;
}

void UpdateEntityList(const std::vector<Entity*>& entityList)
{
	for (Entity* entity : entityList)
	{
		entity->Update(0);
	}
}

void World::Update()
{
	UpdateEntityList(m_Entities);
	UpdateEntityList(m_DebugEntities);

	for (Sector* sector : m_Sectors)
	{
		sector->Update();
	}
}


bool fuzzyEquals2(glm::vec2 a, glm::vec2 b, float tolerance)
{
	return (fabs(a.x - b.x) < tolerance) && (fabs(a.y - b.y) < tolerance);
}

//Sector* World::GetSector(Coord2 longLat)
//{
//	Sector* toReturn = nullptr;
//	for (Sector* sector : m_Sectors)
//	{
//		if (fuzzyEquals2(sector->GetPosition(), longLat, 1e-2f))
//		{
//			return sector;
//		}
//	}
//
//	return nullptr;
//}


Sector* World::GetSectorByIndex(const glm::ivec2& normalizedSectorIndex)
{
	for (Sector* sector : m_Sectors)
	{
		if (sector->GetOffsetIndex() == normalizedSectorIndex)
		{
			return sector;
		}
	}

	return nullptr;
}

Sector* World::GetSectorByWorldPosition(const glm::vec3& worldPosition)
{
	glm::ivec2 normalizedSectorIndex = Sector::GetNormalizedSectorIndex(glm::vec2(worldPosition));

	glm::vec2 earthStartCoords = Sector::GetStartPosition(glm::vec2(worldPosition));
	glm::vec2 worldStartCoords = Sector::EarthToWorld(earthStartCoords);

	return GetSectorByIndex(normalizedSectorIndex);
}



void World::ResetSectors()
{
	for (Sector* sector : m_Sectors)
	{
		delete sector;
	}

	m_Sectors.clear();
}


float World::SampleHeight(const glm::vec3& worldPos)
{
	glm::vec2 normalizedPos = glm::vec2(worldPos) / g_EngineSettings->GetWorldScale();
	glm::ivec2 sectorIndex = glm::ivec2(normalizedPos);

	Sector* sector = GetSectorByIndex(sectorIndex);

	if (sector == nullptr)
	{
		return 0.f;
	}

	return sector->SampleHeight(glm::fract(normalizedPos));
}
