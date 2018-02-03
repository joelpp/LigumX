#include "stdafx.h"
#include "sector.h"
#include "heightfield.h"
#include "LigumX.h"
#include "Logging.h"
#include "SectorManager.h"
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
{ "WorldPosition", PIDX_WorldPosition, offsetof(Sector, m_WorldPosition), 0, LXType_glmvec2, false, LXType_None, 0, 0, 0, }, 
{ "EarthPosition", PIDX_EarthPosition, offsetof(Sector, m_EarthPosition), 0, LXType_glmvec2, false, LXType_None, 0, 0, 0, }, 
{ "QuantizedPosition", PIDX_QuantizedPosition, offsetof(Sector, m_QuantizedPosition), 0, LXType_glmivec2, false, LXType_None, 0, 0, 0, }, 
{ "OffsetIndex", PIDX_OffsetIndex, offsetof(Sector, m_OffsetIndex), 0, LXType_glmivec2, false, LXType_None, 0, 0, 0, }, 
{ "Index", PIDX_Index, offsetof(Sector, m_Index), 0, LXType_glmivec2, false, LXType_None, 0, 0, 0, }, 
{ "DataLoaded", PIDX_DataLoaded, offsetof(Sector, m_DataLoaded), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "OSMFilename", PIDX_OSMFilename, offsetof(Sector, m_OSMFilename), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
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
}


Sector::Sector(vec2 pos, float size, int ID)
	: m_DataLoaded(false)
{
    this->m_LifeSize = glm::vec2(size);

	m_Data = 0;
   	m_heightfield = 0;
   	m_initialized = false;
    m_initializationLevel = Uninitialized;
    m_ID = ID;
}

glm::vec2 Sector::EarthToWorld(const glm::vec2& earthPosition)
{
	const float& worldScale = g_EngineSettings->GetWorldScale();
	const glm::vec2& startEarthPosition = g_EngineSettings->GetStartLonLat();

	glm::ivec2 quantizedPosition = g_SectorManager->EarthToQuantized(earthPosition);
	glm::ivec2 startQuantizedPosition = g_SectorManager->EarthToQuantized(startEarthPosition);

	glm::ivec2 adjustedQuantizedPosition = quantizedPosition - startQuantizedPosition;

	float quantizedExtentF = (float)g_SectorManager->EarthToQuantized(g_EngineSettings->GetExtent());

	glm::vec2 worldPosition = worldScale * (glm::vec2)adjustedQuantizedPosition / quantizedExtentF;

	return worldPosition;
}

Sector::Sector(CurlRequest* curlRequest)
	: m_DataLoaded(false)
{
	m_ObjectID = g_ObjectManager->GetTransientID();

	m_Data = new SectorData();

	m_LifeSize = curlRequest->GetExtent();

	SetEarthPosition(curlRequest->GetCoords());
	SetQuantizedPosition(g_SectorManager->EarthToQuantized(m_EarthPosition));

	SetWorldPosition(EarthToWorld(m_EarthPosition));

	SetOffsetIndex(curlRequest->GetSectorIndex());

	SetOSMFilename(curlRequest->GetFilename());

	curlRequest->SetSector(this);
}

Sector::Sector(const glm::ivec2& index)
{
	m_ObjectID = g_ObjectManager->GetTransientID();

	m_Data = new SectorData();

	m_OffsetIndex = index;
	m_WorldPosition = g_SectorManager->OffsetIndexToWorldPosition(m_OffsetIndex);

	m_LifeSize = glm::vec2(g_EngineSettings->GetExtent());

	m_OSMFilename = "";
}


glm::ivec2 Sector::GetNormalizedSectorIndex(const glm::vec2& position)
{
	glm::vec2 normalizedWorldPosition = position / g_EngineSettings->GetWorldScale();

	glm::ivec2 normalizedSectorIndex = (glm::ivec2) glm::floor(normalizedWorldPosition);

	return normalizedSectorIndex;
}

glm::vec2 Sector::GetStartPosition(glm::vec2 position)
{
	glm::vec2 startCoords = g_EngineSettings->GetStartLonLat();
	glm::vec2 extent = glm::vec2(g_EngineSettings->GetExtent());

	glm::ivec2 normalizedSectorIndex = Sector::GetNormalizedSectorIndex(position);
	startCoords += extent * (glm::vec2)normalizedSectorIndex;

	return startCoords;
}

bool Sector::createHeightfield()
{
    if (!m_heightfield)
    {
        m_heightfield = new Heightfield(m_EarthPosition, m_EarthPosition.x);
    }

    return m_heightfield->generate();
}


void Sector::InitializeFromRequest(CurlRequest* request)
{
	//m_LifeSize = request->GetExtent();

	//SetEarthPosition(request->GetCoords());

	//SetQuantizedPosition(g_SectorManager->EarthToQuantized(m_EarthPosition));

	//SetWorldPosition(EarthToWorld(m_EarthPosition));

	//SetOffsetIndex(request->GetSectorIndex());

	//SetOSMFilename(request->GetFilename());

	//request->SetSector(this);
}
