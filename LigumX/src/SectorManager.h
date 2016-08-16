#pragma once

// #include <pair>
#include <unordered_map>
#include <vector>
#include "glm/glm.hpp"
#include "Logging.h"

class Sector;


typedef std::pair<int, int> IntPair;
typedef glm::vec2 Coord2;
typedef std::vector<Sector*> SectorList;


class SectorManager
{
public:
	SectorManager();
	SectorManager(float m_sectorSize);

	static int IDFromPos(glm::vec2 pos);
	static long int getSectorIndex(int x, int y);

	glm::vec2 posFromID(int ID);
	glm::vec2 posFromVec2(glm::vec2 pos);
	static IntPair intPairFromVec2(glm::vec2 pos);


	Sector* createSector(int ID);
    Sector* createSector(Coord2 pos);
    
    /**
     * [Normalized Takes in a World-Space point and returns the coordinates (on a normalized grid) of the sector holding it]
     * @param  UnNormalized [The WS position to check against]
     * @return              [The normalized coordinates of the sector's "lower-left" corner]
     */
    Coord2 Normalized(Coord2 UnNormalized);

    /**
     * [sectorPosFromXY Takes in a WS point and returns the WS coordinates of the sector holding it]
     * @param  longLat [The WS position to check against]
     * @return              [The WS coordinates of the sector's "lower-left" corner]
     */
    Coord2 sectorPosFromXY(Coord2 longLat);

    /**
     * [SectorFromXY Returns the sector if it exists, or creates it then returns it.]
     * @param  longLat [The position of a point in the sector.]
     * @return     [A pointer to the sector.]
     */
    Sector* sectorContaining(Coord2 longLat);
    Sector* GetOrCreateSectorAtXY(Coord2 longLat, SectorList* newSectors);

    SectorList* sectorsAround(Coord2 point, int ringSize, bool initialization);
    SectorList* newSectorsAroundPoint(Coord2 point, int ringSize);

	void keepInitializing(Sector* sector, int manhattanDistance);


	std::unordered_map<int, Sector*> m_sectors;

	float m_sectorSize;


private:
	REGISTERCLASS(SectorManager);
};