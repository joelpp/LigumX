#include "stdafx.h"

#include "sector.h"
#include "heightfield.h"
#include "LigumX.h"
#include "Logging.h"
#include "RenderDataManager.h"
#include "CurlRequest.h"

using namespace glm;
Sector::Sector()
{
}

Sector::Sector(vec2 pos, float size, int ID)
{
	//PRINT("CONSTRUCTING NEW SECTOR AT")
	//PRINTVEC2(pos);
    this->m_Pos = pos;
    this->m_Size = glm::vec2(size);

	m_Data = 0;
   	m_heightfield = 0;
   	m_initialized = false;
    m_initializationLevel = Uninitialized;
    m_ID = ID;


	m_Data = new SectorData(m_Pos);
}

Sector::Sector(CurlRequest* curlRequest)
{
	m_Data = new SectorData(curlRequest);

	m_Pos = curlRequest->GetCoords();
	m_Size = curlRequest->GetExtent();
}


void Sector::loadData(SectorData::EOSMDataType type)
{
	m_Data->loadData(type);
}

bool Sector::createHeightfield()
{
    if (!m_heightfield)
    {
        m_heightfield = new Heightfield(m_Pos, m_Size.x);
    }

    return m_heightfield->generate();
}