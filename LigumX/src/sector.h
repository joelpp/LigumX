#pragma once
#include "glm/glm.hpp"
#include "SectorData.h"
#include "Building.h"

#pragma region  FORWARD_DECLARATIONS Sector
#include "property.h"

class Sector;
class Heightfield;
class Entity;


#pragma endregion  FORWARD_DECLARATIONS Sector
class SectorData;
class Heightfield;
class vec2i;
class CurlRequest;
class SectorGraphicalData;
class PerlinNoise;
class Texture;
class Entity;

typedef std::pair<int, int> IntPair;

class Sector
{
#pragma region  HEADER Sector
public:
static const int ClassID = 749113007;
static const LXType Type = LXType_Sector;
static constexpr const char* ClassName = "Sector";

int GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
glm::vec2& GetWorldPosition() { return m_WorldPosition; }; 
void SetWorldPosition(const glm::vec2& value) { m_WorldPosition = value; }; 
glm::vec2& GetEarthPosition() { return m_EarthPosition; }; 
void SetEarthPosition(const glm::vec2& value) { m_EarthPosition = value; }; 
glm::ivec2& GetQuantizedPosition() { return m_QuantizedPosition; }; 
void SetQuantizedPosition(const glm::ivec2& value) { m_QuantizedPosition = value; }; 
glm::ivec2& GetOffsetIndex() { return m_OffsetIndex; }; 
void SetOffsetIndex(const glm::ivec2& value) { m_OffsetIndex = value; }; 
bool GetDataLoaded() { return m_DataLoaded; }; 
void SetDataLoaded(bool value) { m_DataLoaded = value; }; 
Heightfield*& GetHeightfield() { return m_Heightfield; }; 
void SetHeightfield(Heightfield* value) { m_Heightfield = value; }; 
Entity*& GetTerrainPatchEntity() { return m_TerrainPatchEntity; }; 
void SetTerrainPatchEntity(Entity* value) { m_TerrainPatchEntity = value; }; 
const std::string& GetOSMFilename() { return m_OSMFilename; }; 
void SetOSMFilename(std::string value) { m_OSMFilename = value; }; 
private:
int m_ObjectID;
std::string m_Name;
glm::vec2 m_WorldPosition = glm::vec2(0, 0);
glm::vec2 m_EarthPosition = glm::vec2(0, 0);
glm::ivec2 m_QuantizedPosition = glm::ivec2(0, 0);
glm::ivec2 m_OffsetIndex = glm::ivec2(0, 0);
bool m_DataLoaded = false;
Heightfield* m_Heightfield = nullptr;
Entity* m_TerrainPatchEntity = nullptr;
std::string m_OSMFilename;
public:
static const int g_PropertyCount = 10;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_SectorPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_WorldPosition,
PIDX_EarthPosition,
PIDX_QuantizedPosition,
PIDX_OffsetIndex,
PIDX_DataLoaded,
PIDX_Heightfield,
PIDX_TerrainPatchEntity,
PIDX_OSMFilename,
};
bool Serialize(bool writing);

#pragma endregion  HEADER Sector



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
	Sector(CurlRequest* curlRequest);
	Sector(const glm::ivec2& index);

	~Sector();

	void Update();

	static glm::ivec2 GetNormalizedSectorIndex(const glm::vec2& position);
	static glm::vec2 GetStartPosition(glm::vec2 position);

	SectorData *m_Data;

	glm::vec2 m_LifeSize;
	glm::vec2 m_Size;

    EInitializationLevel m_initializationLevel;
    bool m_initialized;
    int m_ID;

	static glm::vec2 EarthToWorld(const glm::vec2& earthPosition);

	void InitializeFromRequest(CurlRequest* request);
	void CreateHeightfield();

	SectorGraphicalData* GetGraphicalData() { return m_Data->GetGraphicalData(); }
	Texture* GetSplatMapTexture();

	float SampleHeight(const glm::vec2& normalizedPos);
	float SampleHeight(const glm::vec3& worldPos);

	glm::vec2 GetUVForWorldPosition(const glm::vec3& worldPosition);
	glm::vec3 GetWorldPositionForUV(const glm::vec2& uv);

	std::vector<Building>& GetBuildings() { return m_Buildings; }
	private:
		std::vector<Building> m_Buildings;
};
