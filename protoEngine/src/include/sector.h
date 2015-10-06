#ifndef CHUNK
#define CHUNK

#include "glm/glm.hpp"

class SectorData;
class Heightfield;

class Sector{

public:
	SectorData *m_data;
    Heightfield *m_heightfield;
    glm::vec2 m_pos;

    Sector();
    Sector(glm::vec2 pos, float size);
    void loadData();
    void heightfieldTesting();

    
};

#endif // CHUNK

