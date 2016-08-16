#include "stdafx.h"

#include "sector.h"
#include "heightfield.h"
#include "LigumX.h"
#include "Logging.h"
#include "RenderDataManager.h"

using namespace glm;
Sector::Sector()
{
}

Sector::Sector(vec2 pos, float size, int ID)
{
	PRINT("CONSTRUCTING NEW SECTOR AT")
	PRINTVEC2(pos);
    this->m_pos = pos;
    this->m_size = glm::vec2(size);
    m_data = 0;
   	m_heightfield = 0;
   	m_initialized = false;
    m_initializationLevel = Uninitialized;
    m_ID = ID;
	// TODO: put this elsewhere holy shit
    // LigumX::GetInstance().renderData->initializeSector(this);
    
    // createHeightfield();
            //     // renderData->addToTerrainBuffer(sector);
            //     // sector->loadData(SectorData::MAP);
            //     // sector->m_data->elevateNodes(sector->m_heightfield);
    // LigumX::GetInstance().renderData->fillBuffers(this);
    m_data = new SectorData(m_pos);
}

void Sector::loadData(SectorData::EOSMDataType type){
    if (!m_data)
    {
    	// m_data = new SectorData(m_pos);
    } 
    m_data->loadData(type);
}

bool Sector::createHeightfield()
{
    if (!m_heightfield)
    {
        m_heightfield = new Heightfield(m_pos, m_size.x);
    }

    return m_heightfield->generateTriangles();
}