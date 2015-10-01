#ifndef CHUNK
#define CHUNK

#include "glm/glm.hpp"
#include "heightfield.h"
#include "osm_element.h"
#include "node.h"
#include "way.h"
#include "relation.h"
#include "SectorData.h"

class Sector{
public:
	SectorData m_data;
    glm::vec2 m_pos;
    Heightfield* m_heightfield;

    Sector();
    Sector(glm::vec2 pos);
    void loadData();
    void heightfieldTesting();

    
};

#endif // CHUNK

