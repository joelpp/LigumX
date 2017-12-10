#include "stdafx.h"
#include "sector.h"
#include "heightfield.h"
#include "LigumX.h"
#include "Logging.h"
#include "RenderDataManager.h"
#include "EngineSettings.h"
#include "CurlRequest.h"

#pragma region  CLASS_SOURCE Sector

#include "Sector.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData Sector::g_Properties[] = 
{
{ "ObjectID", PIDX_ObjectID, offsetof(Sector, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "Name", PIDX_Name, offsetof(Sector, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
{ "Position", PIDX_Position, offsetof(Sector, m_Position), 0, LXType_glmvec2, false, LXType_None, 0, 0, 0, }, 
{ "WorldPosition", PIDX_WorldPosition, offsetof(Sector, m_WorldPosition), 0, LXType_glmvec2, false, LXType_None, 0, 0, 0, }, 
{ "EarthPosition", PIDX_EarthPosition, offsetof(Sector, m_EarthPosition), 0, LXType_glmvec2, false, LXType_None, 0, 0, 0, }, 
{ "QuantizedPosition", PIDX_QuantizedPosition, offsetof(Sector, m_QuantizedPosition), 0, LXType_glmivec2, false, LXType_None, 0, 0, 0, }, 
{ "OffsetIndex", PIDX_OffsetIndex, offsetof(Sector, m_OffsetIndex), 0, LXType_glmvec2, false, LXType_None, 0, 0, 0, }, 
{ "Index", PIDX_Index, offsetof(Sector, m_Index), 0, LXType_glmivec2, false, LXType_None, 0, 0, 0, }, 
{ "DataLoaded", PIDX_DataLoaded, offsetof(Sector, m_DataLoaded), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
};
bool Sector::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}

#pragma endregion  CLASS_SOURCE Sector

#define OSM_QUANTIZATION_SCALE 1e7f

using namespace glm;
Sector::Sector()
	: m_DataLoaded(false)
{
}

Sector::Sector(glm::vec2 startPos)
	: m_DataLoaded(false)
{
	m_Position = startPos;

	if (!m_Data)
	{
		m_Data = new SectorData(m_Position);
	}
}


Sector::Sector(vec2 pos, float size, int ID)
	: m_DataLoaded(false)
{
	//PRINT("CONSTRUCTING NEW SECTOR AT")
	//PRINTVEC2(pos);
    this->m_Position = pos;
    this->m_LifeSize = glm::vec2(size);

	m_Data = 0;
   	m_heightfield = 0;
   	m_initialized = false;
    m_initializationLevel = Uninitialized;
    m_ID = ID;


	m_Data = new SectorData(m_Position);
}

glm::ivec2 EarthToQuantizedPosition(const glm::vec2& earthPosition)
{
	return (glm::ivec2) (earthPosition * OSM_QUANTIZATION_SCALE);
}

Sector::Sector(CurlRequest* curlRequest)
	: m_DataLoaded(false)
{
	m_Data = new SectorData(curlRequest);

	m_Position = curlRequest->GetCoords();
	m_LifeSize = curlRequest->GetExtent();

	SetEarthPosition(curlRequest->GetCoords());
	SetQuantizedPosition(EarthToQuantizedPosition(m_EarthPosition));

	const glm::vec2& startEarthPosition = g_EngineSettings->GetStartLonLat();
	glm::ivec2 startQuantizedPosition = EarthToQuantizedPosition(startEarthPosition);

	glm::ivec2 adjustedQuantizedPosition = m_QuantizedPosition - startQuantizedPosition;

	float quantizedExtentF = g_EngineSettings->GetExtent() * OSM_QUANTIZATION_SCALE;

	glm::vec2 worldPosition = (glm::vec2)adjustedQuantizedPosition / quantizedExtentF;
	SetWorldPosition(worldPosition);
}


glm::vec2 Sector::GetNormalizedSectorIndex(glm::vec2 position)
{
	glm::vec2 normalizedWorldPosition = position / g_EngineSettings->GetWorldScale();

	glm::vec2 normalizedSectorIndex = glm::floor(normalizedWorldPosition);

	return normalizedSectorIndex;
}

glm::vec2 Sector::GetStartPosition(glm::vec2 position)
{
	glm::vec2 startCoords = g_EngineSettings->GetStartLonLat();
	glm::vec2 extent = glm::vec2(g_EngineSettings->GetExtent());
	startCoords -= extent / 2.f;

	glm::vec2 normalizedSectorIndex = Sector::GetNormalizedSectorIndex(position);
	startCoords += extent * normalizedSectorIndex;

	return startCoords;
}

void Sector::loadData(SectorData::EOSMDataType type)
{
	m_Data->loadData(type);

	m_DataLoaded = true;
}

bool Sector::createHeightfield()
{
    if (!m_heightfield)
    {
        m_heightfield = new Heightfield(m_Position, m_Position.x);
    }

    return m_heightfield->generate();
}
