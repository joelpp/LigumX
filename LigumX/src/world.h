#ifndef WORLD
#define WORLD

#include <unordered_map>
#include <sstream>
#include <vector>

#include "glm/glm.hpp"
#include "tinyxml2.h"

#include "Logging.h"

#pragma region  FORWARD_DECLARATIONS World
#include "property.h"
class SunLight;
class Entity;

#pragma endregion  FORWARD_DECLARATIONS World
class Sunlight;
class Sector;
class Way;
class Node;
class SectorManager;
class vec2i;
class Entity;
class SunLight;

typedef glm::vec2 Coord2;
typedef std::vector<Sector*> SectorList;

//struct Sun{
//    float orientation = 0;
//    float speed = 0.1;
//    bool moveAuto = false;
//};

//Specialize std::hash for vec2s
namespace std {
    template <>
        class hash<glm::vec2>{
        public :
            size_t operator()(const glm::vec2 &vec ) const
            {
                return hash<int>()(roundf(vec.x)) ^ hash<int>()(roundf(vec.y));
            }
    };
};

class World
{
#pragma region  HEADER World
public:
static const int ClassID = 3714116915;
const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
SunLight* GetSunLight() { return m_SunLight; }; 
void SetSunLight(SunLight* value) { m_SunLight = value; }; 
const std::vector<Entity*>& GetEntities() { return m_Entities; }; 
void SetEntities(std::vector<Entity*> value) { m_Entities = value; }; 
private:
int m_ObjectID;
SunLight* m_SunLight;
std::vector<Entity*> m_Entities;
public:
static const int g_PropertyCount = 3;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_WorldPIDX
{
PIDX_ObjectID,
PIDX_SunLight,
PIDX_Entities,
};
void Serialize(bool writing);

#pragma endregion  HEADER World

private:
    REGISTERCLASS(World);
    
public:

    World();
    World(float sectorSize);

	void InitTestWorld();

	void Update();

    Sector* createSector(Coord2 pos);
	SectorList* GetAllSectors();

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
    Sector* GetOrCreateSectorContainingXY(Coord2 longLat);
    Sector* GetOrCreateSectorAtXY(Coord2 longLat, SectorList* newSectors);

    SectorList* updateSectorsAroundPoint(Coord2 point, int ringSize);
    SectorList* sectorsAroundPoint(Coord2 point, int ringSize);
    SectorList* newSectorsAroundPoint(Coord2 point, int ringSize);

    static std::vector<Way*> findNClosestWays(int n, glm::vec2 xy, 
                                       std::vector<Way*> querySet, int filter, 
                                       std::vector<double> &distances, 
                                       std::vector<glm::vec2> &_directions, 
                                       std::vector<std::pair<Node*, Node*>> &_nodePairs);
    
    static std::vector<Way*> getAllContourLines(SectorList* sectors);
    
    float m_sectorSize;
    float m_invSectorSize;
    std::unordered_map<Coord2, Sector*> m_sectors;
    std::vector<Sector*> sectors;
    SectorManager* m_sectorManager;
    float m_globalTime;

};
#endif

// 45.5 -73.65 45.55 -73.6
// 45.55 -73.6 45.6 -73.55
