
#include "LigumX.h"
#include "SectorManager.h"
#include "SectorData.h"
#include "SectorGraphicalData.h"
#include "RenderDataManager.h"
#include "EngineSettings.h"
#include "CurlRequest.h"
#include "PerlinNoise.h"
#include "BoundingBoxComponent.h"

#pragma region  CLASS_SOURCE Sector

#include "Sector.h"
#include "serializer.h"
#include "Heightfield.h"
#include "Entity.h"
const ClassPropertyData Sector::g_Properties[] = 
{
{ "WorldPosition", PIDX_WorldPosition, offsetof(Sector, m_WorldPosition), 0, LXType_glmvec2, sizeof(glm::vec2), LXType_glmvec2, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "EarthPosition", PIDX_EarthPosition, offsetof(Sector, m_EarthPosition), 0, LXType_glmvec2, sizeof(glm::vec2), LXType_glmvec2, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "QuantizedPosition", PIDX_QuantizedPosition, offsetof(Sector, m_QuantizedPosition), 0, LXType_glmivec2, sizeof(glm::ivec2), LXType_glmivec2, false, LXType_None, false, 0, (float)LX_LIMITS_INT_MIN, (float)LX_LIMITS_INT_MAX, 0,}, 
{ "OffsetIndex", PIDX_OffsetIndex, offsetof(Sector, m_OffsetIndex), 0, LXType_glmivec2, sizeof(glm::ivec2), LXType_glmivec2, false, LXType_None, false, 0, (float)LX_LIMITS_INT_MIN, (float)LX_LIMITS_INT_MAX, 0,}, 
{ "DataLoaded", PIDX_DataLoaded, offsetof(Sector, m_DataLoaded), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "Heightfield", PIDX_Heightfield, offsetof(Sector, m_Heightfield), 0, LXType_ObjectPtr, sizeof(Heightfield*), LXType_Heightfield, true, LXType_None, false, PropertyFlags_Transient, 0, 0, 0,}, 
{ "TerrainPatchEntity", PIDX_TerrainPatchEntity, offsetof(Sector, m_TerrainPatchEntity), 0, LXType_ObjectPtr, sizeof(Entity*), LXType_Entity, true, LXType_None, false, PropertyFlags_Transient, 0, 0, 0,}, 
{ "OSMFilename", PIDX_OSMFilename, offsetof(Sector, m_OSMFilename), 0, LXType_stdstring, sizeof(std::string), LXType_stdstring, false, LXType_None, false, 0, 0, 0, 0,}, 
};
void Sector::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeVec2(g_Properties[PIDX_WorldPosition], m_WorldPosition);
	serializer.SerializeVec2(g_Properties[PIDX_EarthPosition], m_EarthPosition);
	serializer.SerializeIVec2(g_Properties[PIDX_QuantizedPosition], m_QuantizedPosition);
	serializer.SerializeIVec2(g_Properties[PIDX_OffsetIndex], m_OffsetIndex);
	serializer.SerializeBool(g_Properties[PIDX_DataLoaded], m_DataLoaded);
	serializer.SerializeString(g_Properties[PIDX_OSMFilename], m_OSMFilename);
}
bool Sector::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool Sector::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_WorldPosition], &m_WorldPosition , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_EarthPosition], &m_EarthPosition , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_QuantizedPosition], &m_QuantizedPosition , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_OffsetIndex], &m_OffsetIndex , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_DataLoaded], &m_DataLoaded  );
	ImguiHelpers::ShowObject2(this, g_Properties[PIDX_Heightfield], &m_Heightfield  );
	ImguiHelpers::ShowObject2(this, g_Properties[PIDX_TerrainPatchEntity], &m_TerrainPatchEntity  );
	ImguiHelpers::ShowString2(this, g_Properties[PIDX_OSMFilename], m_OSMFilename  );
	return true;
}
void Sector::Clone(LXObject* otherObj)
{
	super::Clone(otherObj);
	Sector* other = (Sector*) otherObj;
	other->SetWorldPosition(m_WorldPosition);
	other->SetEarthPosition(m_EarthPosition);
	other->SetQuantizedPosition(m_QuantizedPosition);
	other->SetOffsetIndex(m_OffsetIndex);
	other->SetDataLoaded(m_DataLoaded);
	other->SetHeightfield(m_Heightfield);
	other->SetTerrainPatchEntity(m_TerrainPatchEntity);
	other->SetOSMFilename(m_OSMFilename);
}
const char* Sector::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE Sector

#define OSM_QUANTIZATION_SCALE 1e7f

using namespace glm;

Sector::Sector()
{
	// Not supposed to call the default Sector constructor! >:(
	lxAssert0();
}

Sector::~Sector()
{
	delete m_Data;

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
	SetObjectID(g_ObjectManager->GetTransientID());

	m_Data = new SectorData();

	m_LifeSize = curlRequest->GetExtent();

	SetOffsetIndex(curlRequest->GetSectorIndex());
	SetEarthPosition(curlRequest->GetCoords());
	SetQuantizedPosition(g_SectorManager->EarthToQuantized(m_EarthPosition));

	SetWorldPosition(g_SectorManager->OffsetIndexToWorldPosition(m_OffsetIndex));


	SetOSMFilename(curlRequest->GetFilename());

	curlRequest->SetSector(this);

	CreateHeightfield();

}

Sector::Sector(const glm::ivec2& index)
{
	SetObjectID(g_ObjectManager->GetTransientID());

	m_Data = new SectorData();

	m_OffsetIndex = index;
	m_WorldPosition = g_SectorManager->OffsetIndexToWorldPosition(m_OffsetIndex);

	m_LifeSize = glm::vec2(g_EngineSettings->GetExtent());

	m_OSMFilename = "";

	CreateHeightfield();
}

void UpdateEntityList2(const std::vector<Entity*>& entityList)
{
	for (Entity* entity : entityList)
	{
		entity->Update(0);
	}
}

void Sector::Update()
{
	UpdateEntityList2(m_Data->GetGraphicalData()->GetRoadEntities());
	UpdateEntityList2(m_Data->GetGraphicalData()->GetStaticEntities());
}

void Sector::CreateHeightfield()
{
	m_Heightfield = new Heightfield(glm::vec2(m_OffsetIndex));

	m_TerrainPatchEntity = new Entity();

	glm::mat4 mat = glm::mat4(1.0);
	glm::vec3 scale = glm::vec3(g_EngineSettings->GetWorldScale(), g_EngineSettings->GetWorldScale(), 4.f);
	glm::vec3 worldPosition = glm::vec3(m_WorldPosition, 0);
	mat = glm::translate(mat, worldPosition);
	mat = glm::scale(mat, scale);
	m_TerrainPatchEntity->SetModelToWorldMatrix(mat);

	std::stringstream name;
	name << "Terrain Patch [";
	name << m_OffsetIndex.x << ", " << m_OffsetIndex.y << "]";
	m_TerrainPatchEntity->SetName(name.str());
	
	Material* terrainMaterial = new Material();
	terrainMaterial->SetHeightfieldTexture(m_Heightfield->GetHeightDataTexture());

	Texture* grassTexture = g_ObjectManager->GetLXObject<Texture>(10691);
	terrainMaterial->SetDiffuseTexture(grassTexture);

	Model* terrainPatchModel = new Model(g_DefaultObjects->DefaultTerrainMesh, terrainMaterial);
	m_TerrainPatchEntity->SetModel(terrainPatchModel);

	BoundingBoxComponent* bb = m_TerrainPatchEntity->GetComponent<BoundingBoxComponent>();
	bb->SetStartAndScale(worldPosition, scale);
}


glm::ivec2 Sector::GetNormalizedSectorIndex(const glm::vec2& position)
{
	glm::vec2 normalizedWorldPosition = position / g_EngineSettings->GetWorldScale();

	glm::ivec2 normalizedSectorIndex = (glm::ivec2) glm::floor(normalizedWorldPosition);

	return normalizedSectorIndex;
}

glm::vec2 Sector::GetStartPosition(glm::vec2 position, glm::ivec2 sectorIndex)
{
	glm::dvec2 startCoords = glm::dvec2(g_EngineSettings->GetStartLonLat());
	glm::dvec2 extent = glm::dvec2((double)g_EngineSettings->GetExtent());

	glm::ivec2 normalizedSectorIndex = sectorIndex;
	startCoords += extent * (glm::dvec2)normalizedSectorIndex;

	return glm::vec2(startCoords);
}

glm::vec2 Sector::GetStartPosition(glm::vec2 position)
{
	glm::ivec2 normalizedSectorIndex = Sector::GetNormalizedSectorIndex(position);
	return GetStartPosition(position, normalizedSectorIndex);
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

float Sector::SampleHeight(const glm::vec2& normalizedPos)
{
	lxAssert(m_Heightfield != nullptr);

	if (g_EngineSettings->GetGenerateFlatTerrain())
	{
		return 0.f;
	}

	return m_Heightfield->SampleHeight(normalizedPos);
}

float Sector::SampleHeight(const glm::vec3& worldPos)
{
	lxAssert(m_Heightfield != nullptr);

	glm::vec2 sectorOffset = glm::vec2(worldPos) - m_WorldPosition;

	glm::vec2 normalizedPos = sectorOffset / g_EngineSettings->GetWorldScale();

	return SampleHeight(normalizedPos);
}

Texture* Sector::GetSplatMapTexture()
{
	lxAssert(m_Data->GetGraphicalData());

	return m_Data->GetGraphicalData()->GetSplatMapTexture();
}


glm::vec2 Sector::GetUVForWorldPosition(const glm::vec3& worldPosition)
{
	glm::vec2 uv = /*g_EngineSettings->GetTerrainTiling() * */(glm::vec2(worldPosition) - m_WorldPosition) / g_EngineSettings->GetWorldScale();
	return uv;
}


glm::vec3 Sector::GetWorldPositionForUV(const glm::vec2& uv)
{
	return glm::vec3(g_EngineSettings->GetWorldScale() * uv + m_WorldPosition, 0);
}
