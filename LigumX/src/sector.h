#pragma once
#include "glm/glm.hpp"
#include "SectorData.h"

#pragma region  FORWARD_DECLARATIONS Sector
#include "property.h"

class Sector;


#pragma endregion  FORWARD_DECLARATIONS Sector
class SectorData;
class Heightfield;
class vec2i;
class CurlRequest;
typedef std::pair<int, int> IntPair;


class Sector
{
#pragma region  HEADER Sector
public:
static const int ClassID = 749113007;
static const LXType Type = LXType_Sector;
static constexpr const char* ClassName = "Sector";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
const glm::vec2& GetPosition() { return m_Position; }; 
void SetPosition(glm::vec2 value) { m_Position = value; }; 
const glm::vec2& GetWorldPosition() { return m_WorldPosition; }; 
void SetWorldPosition(glm::vec2 value) { m_WorldPosition = value; }; 
const glm::vec2& GetEarthPosition() { return m_EarthPosition; }; 
void SetEarthPosition(glm::vec2 value) { m_EarthPosition = value; }; 
const glm::ivec2& GetQuantizedPosition() { return m_QuantizedPosition; }; 
void SetQuantizedPosition(glm::ivec2 value) { m_QuantizedPosition = value; }; 
const glm::ivec2& GetOffsetIndex() { return m_OffsetIndex; }; 
void SetOffsetIndex(glm::ivec2 value) { m_OffsetIndex = value; }; 
const glm::ivec2& GetIndex() { return m_Index; }; 
void SetIndex(glm::ivec2 value) { m_Index = value; }; 
const bool& GetDataLoaded() { return m_DataLoaded; }; 
void SetDataLoaded(bool value) { m_DataLoaded = value; }; 
private:
int m_ObjectID;
std::string m_Name;
glm::vec2 m_Position = glm::vec2(0, 0);
glm::vec2 m_WorldPosition = glm::vec2(0, 0);
glm::vec2 m_EarthPosition = glm::vec2(0, 0);
glm::ivec2 m_QuantizedPosition;
glm::ivec2 m_OffsetIndex;
glm::ivec2 m_Index;
bool m_DataLoaded = false;
public:
static const int g_PropertyCount = 9;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_SectorPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_Position,
PIDX_WorldPosition,
PIDX_EarthPosition,
PIDX_QuantizedPosition,
PIDX_OffsetIndex,
PIDX_Index,
PIDX_DataLoaded,
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
    Sector(glm::vec2 pos, float size, int ID);
	Sector(vec2i pos, float size);
	Sector(CurlRequest* curlRequest);
	Sector(const glm::ivec2& index);
	Sector(glm::vec2 startPos);
    bool createHeightfield();

	static glm::vec2 OffsetIndexToWorldPosition(const glm::ivec2& sectorIndex);
	static glm::ivec2 GetNormalizedSectorIndex(const glm::vec2& position);
	static glm::vec2 GetStartPosition(glm::vec2 position);

	SectorData *m_Data;

    Heightfield *m_heightfield;
	glm::vec2 m_LifeSize;
	glm::vec2 m_Size;

    EInitializationLevel m_initializationLevel;
    bool m_initialized;
    int m_ID;

	static glm::vec2 EarthToWorld(const glm::vec2& earthPosition);

};
