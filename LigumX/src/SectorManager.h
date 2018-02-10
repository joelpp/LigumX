#pragma once

// #include <pair>
#include <map>
#include <unordered_map>
#include <vector>
#include "glm/glm.hpp"
#include "Logging.h"
#include "SectorData.h"




class SectorData;
class Sector;
class World;
class CurlRequest;
class SectorManager;

extern SectorManager* g_SectorManager;

typedef std::pair<int, int> IntPair;
typedef glm::vec2 Coord2;
typedef std::vector<Sector*> SectorList;

class SectorManager
{
public:
	SectorManager();
	SectorManager(float m_sectorSize);

	int IDFromPos(glm::vec2 pos);
	int getSectorIndex(float x, float y);

	glm::vec2 posFromID(int ID);
	glm::vec2 posFromVec2(glm::vec2 pos);
	static IntPair intPairFromVec2(glm::vec2 pos);


	Sector* createSector(int ID);
    Sector* createSector(Coord2 pos);
	SectorList GetAllSectors();


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
    
    Sector* getSector(int ID);
    
    
    Sector* GetOrCreateSectorAtXY(Coord2 longLat, SectorList* newSectors);

    SectorList* sectorsAround(Coord2 point, int ringSize, bool initialization);
    SectorList* newSectorsAroundPoint(Coord2 point, int ringSize);

	void keepInitializing(Sector* sector, int manhattanDistance);


	std::unordered_map<int, Sector*> m_Sectors;

	float m_sectorSize;

	Node* GetClosestNode(glm::vec2 wsPosition, bool searchOnlyWithinSector);

	void AddSector(Sector* sector);
	Sector* CreateSector(CurlRequest* request);
	Sector* CreateSector(const glm::ivec2& sectorIndex);

	glm::ivec2 EarthToQuantized(const glm::vec2& earthPosition);

	int EarthToQuantized(const float& earthPosition);


	void LoadRequest(CurlRequest* request, SectorData::EOSMDataType dataType);

	void ProcessXMLResult(tinyxml2::XMLNode* docRoot);

	glm::vec2 OffsetIndexToWorldPosition(const glm::ivec2& sectorIndex);



	std::unordered_map<std::string, Node*> m_AllNodes;
	std::map<int, Node*> m_AllNodesPtr;
	std::map<std::string, Way*> m_AllWays;
	std::map<int, Way*> m_AllWaysPtr;

private:
	REGISTERCLASS(SectorManager);
    
    int iStartX, iStartY;
    int iNumSectorsX, iNumSectorsY;
    int iSizeX, iSizeY;

	World* m_World;


};