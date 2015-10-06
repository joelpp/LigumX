#ifndef WORLD
#define WORLD

#include <unordered_map>
#include <sstream>

#include "glm/glm.hpp"
#include "tinyxml2.h"

#include "Logging.h"

class Sector;

typedef glm::vec2 Coord2;

struct Sun{
    float orientation = 0;
    float time = 0;
    float speed = 0.1;
    bool moveAuto = false;
};

//Specialize std::hash for vec2s
namespace std {
    template <>
        class hash<glm::vec2>{
        public :
            size_t operator()(const glm::vec2 &vec ) const
            {
                return hash<int>()(vec.x) ^ hash<int>()(vec.y);
            }
    };
};

class World{
private:
    REGISTERCLASS(World);
    
public:

    World();
    World(float sectorSize);
    Sun sun;
    /** sectors */
    Sector* createSector(Coord2 pos);

    float m_sectorSize;
    float m_invSectorSize;
    std::unordered_map<Coord2, Sector*> m_sectors;

    // Sector* getSector();
    Coord2 sectorPosFromXY(Coord2 longLat);
    Coord2 Normalized(Coord2 longLat);
};
#endif

// 45.5 -73.65 45.55 -73.6
// 45.55 -73.6 45.6 -73.55
