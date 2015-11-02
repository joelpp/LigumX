#include "world.h"
#include "sector.h"
#include "Way.h"
#include "Node.h"
#include "Math.h"
#include "SectorData.h"
#include "vec2i.h"

using namespace std;
using namespace glm;


World::World()
{
}

World::World(float sectorSize)
{
	m_sectorSize = sectorSize;
	m_invSectorSize = 1.f / sectorSize;
}

Sector* World::createSector(Coord2 pos){
	Sector* c = new Sector(pos, m_sectorSize);

    m_sectors.emplace(pos, c);

    return c;
}

Coord2 World::Normalized(Coord2 UnNormalized){
	return UnNormalized * m_invSectorSize;
}

Coord2 World::sectorPosFromXY(Coord2 longLat){
	Coord2 normalized = Normalized(longLat);
	normalized = glm::floor(normalized);

	return normalized * m_sectorSize;
}

Sector* World::GetOrCreateSectorAtXY(Coord2 longLat, SectorList* newSectors){
	Sector* sector;
	longLat.x = roundf(longLat.x * 100) / 100;  /* Result: 37.78 */
	longLat.y = roundf(longLat.y * 100) / 100;  /* Result: 37.78 */
	try{
		sector = m_sectors.at(longLat);
	}
	catch(...){
		PRINT("Creating sector at ");
		PRINTVEC2(longLat);
		sector = new Sector(longLat, m_sectorSize);

    	m_sectors.emplace(longLat, sector);

    	if (newSectors)
    	{
			newSectors->push_back(sector);
		}
	}

	return sector;
}

Sector* World::GetOrCreateSectorContainingXY(Coord2 longLat){
	Coord2 sp = sectorPosFromXY(longLat); 
	sp.x = roundf(sp.x * 100) / 100;  /* Result: 37.78 */
	sp.y = roundf(sp.y * 100) / 100;  /* Result: 37.78 */
	Sector* sector;
	try{
		sector = m_sectors.at(sp);
	}
	catch(...){
		sector = new Sector(sp, m_sectorSize);

    	m_sectors.emplace(sp, sector);
	}

	return sector;
}

SectorList* World::newSectorsAroundPoint(Coord2 point, int ringSize){
	SectorList* newSectors = new SectorList();
	Coord2 startingCoord = sectorPosFromXY(point - glm::vec2(ringSize * m_sectorSize));
	Coord2 runningCoord = startingCoord;;
	for (int i = 0; i < 2 * ringSize + 1; ++i )
	{

		runningCoord.x = startingCoord.x;

		for (int j = 0; j < 2 * ringSize + 1; ++j )
		{

			GetOrCreateSectorAtXY(runningCoord, newSectors);
			runningCoord.x += m_sectorSize;
		}
		runningCoord.y += m_sectorSize;
	}
	return newSectors;
}

SectorList* World::sectorsAroundPoint(Coord2 point, int ringSize){
	SectorList* newSectors = new SectorList();
	Coord2 startingCoord = sectorPosFromXY(point - glm::vec2(ringSize * m_sectorSize));
	Coord2 runningCoord = startingCoord;;
	runningCoord.x = roundf(runningCoord.x * 100) / 100;  /* Result: 37.78 */
	runningCoord.y = roundf(runningCoord.y * 100) / 100;  /* Result: 37.78 */
	for (int i = 0; i < 2 * ringSize + 1; ++i )
	{

		runningCoord.x = startingCoord.x;

		for (int j = 0; j < 2 * ringSize + 1; ++j )
		{
			// PRINTVEC2(runningCoord);

			newSectors->push_back(GetOrCreateSectorAtXY(runningCoord, 0));
			runningCoord.x += m_sectorSize;
		}
		runningCoord.y += m_sectorSize;
	}
	return newSectors;
}

// SectorList* World::sectorsAroundPoint(Coord2 point, int ringSize){
// 	SectorList* newSectors = new SectorList();
// 	Coord2 startingCoord = sectorPosFromXY(point - glm::vec2(ringSize * m_sectorSize));

// 	vec2i sectorPosition = round100();
// 	Coord2 runningCoord = startingCoord;;
// 	for (int i = 0; i < 2 * ringSize + 1; ++i )
// 	{

// 		runningCoord.x = startingCoord.x;

// 		for (int j = 0; j < 2 * ringSize + 1; ++j )
// 		{

// 			newSectors->push_back(GetOrCreateSectorAtXY(runningCoord, 0));
// 			runningCoord.x += m_sectorSize;
// 		}
// 		runningCoord.y += m_sectorSize;
// 	}
// 	return newSectors;
// }

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

        Node* firstNode;
        Node* secondNode;
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