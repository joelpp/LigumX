#pragma once

#include "glm/glm.hpp"
#include "SectorData.h"
class SectorData;
class Heightfield;
class vec2i;

class Sector{

public:
    Sector();
    Sector(glm::vec2 pos, float size);
    Sector(vec2i pos, float size);
    void loadData(SectorData::EOSMDataType type);
    void createHeightfield();

	SectorData *m_data;
    Heightfield *m_heightfield;
    glm::vec2 m_pos;
    glm::vec2 m_size;

    bool m_initialized;

};
