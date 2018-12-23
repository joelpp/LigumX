#pragma once
#include <unordered_map>
#include "glm/glm.hpp"
#include "Logging.h"
#include "OSMElement.h"

#pragma region  FORWARD_DECLARATIONS Node
#include "LXSystem.h"

class Node;
class Way;


#pragma endregion  FORWARD_DECLARATIONS Node

#pragma region  HEADER Node
class Node : public OSMElement
{
public:
static const int ClassID = 879231789;
static const LXType Type = LXType_Node;
static constexpr const char* ClassName = "Node";
typedef OSMElement super;

const long& GetOSMId() { return m_OSMId; }; 
void SetOSMId(long value) { m_OSMId = value; }; 
glm::vec2& GetLongLat() { return m_LongLat; }; 
void SetLongLat(const glm::vec2& value) { m_LongLat = value; }; 
glm::vec3& GetWorldPosition() { return m_WorldPosition; }; 
void SetWorldPosition(const glm::vec3& value) { m_WorldPosition = value; }; 
float GetElevation() { return m_Elevation; }; 
void SetElevation(float value) { m_Elevation = value; }; 
std::vector<Way*>& GetWays() { return m_Ways; }; 
void SetWays(std::vector<Way*> value) { m_Ways = value; }; 
void AddTo_Ways(Way* value) { m_Ways.push_back(value); };
glm::ivec2& GetSectorIndex() { return m_SectorIndex; }; 
void SetSectorIndex(const glm::ivec2& value) { m_SectorIndex = value; }; 
glm::vec2& GetSectorOffset() { return m_SectorOffset; }; 
void SetSectorOffset(const glm::vec2& value) { m_SectorOffset = value; }; 
private:
long m_OSMId;
glm::vec2 m_LongLat = glm::vec2(0, 0);
glm::vec3 m_WorldPosition = glm::vec3(0, 0, 0);
float m_Elevation = 0.f;
std::vector<Way*> m_Ways;
glm::ivec2 m_SectorIndex = glm::ivec2(0, 0);
glm::vec2 m_SectorOffset = glm::vec2(0, 0);
public:
static const int g_PropertyCount = 7;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_NodePIDX
{
PIDX_OSMId,
PIDX_LongLat,
PIDX_WorldPosition,
PIDX_Elevation,
PIDX_Ways,
PIDX_SectorIndex,
PIDX_SectorOffset,
};
bool Serialize(bool writing);
bool ShowPropertyGrid();

#pragma endregion  HEADER Node


public:
    float latitude;
    float longitude;
    double elevation;
    Node();
    Node(glm::vec2 latLong);
    Node(std::string id, float longitude, float latitude);
	Node(std::string _id, float _longitude, float _latitude, float _height);

    std::string toString();
    glm::vec2 getLatLong();
    glm::vec3 getLatLongEle();
    bool operator == (Node& n) const 
	{
       return latitude==n.latitude && longitude==n.longitude;
    }

	void AddWay(Way* way);
};

