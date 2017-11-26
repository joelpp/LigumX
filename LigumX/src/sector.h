#pragma once

#include "glm/glm.hpp"
#include "SectorData.h"
class SectorData;
class Heightfield;
class vec2i;
class CurlRequest;
typedef std::pair<int, int> IntPair;




class Sector{



public:
    enum EInitializationLevel
    {
        Uninitialized,
        ContourLoaded,
        DataLoaded,
        HeightfieldGenerating,
        HeightfieldGenerated,
        FullyInitialized
    };

    Sector();
    Sector(glm::vec2 pos, float size, int ID);
	Sector(vec2i pos, float size);
	Sector(CurlRequest* curlRequest);
    void loadData(SectorData::EOSMDataType type);
    bool createHeightfield();

	SectorData *m_Data;

    Heightfield *m_heightfield;
    glm::vec2 m_Pos;
	glm::vec2 m_LifeSize;
	glm::vec2 m_Size;

    EInitializationLevel m_initializationLevel;
    bool m_initialized;
    int m_ID;
private:
	REGISTERCLASS(Sector);
};
