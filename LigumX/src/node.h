#pragma once
#include <unordered_map>
#include "glm/glm.hpp"
#include "Logging.h"
#include "OSMElement.h"

#pragma region  FORWARD_DECLARATIONS Node
#include "property.h"

class Node;
class Way;


#pragma endregion  FORWARD_DECLARATIONS Node
class Node : public OSMElement
{
#pragma region  HEADER Node
public:
static const int ClassID = 879231789;
static const LXType Type = LXType_Node;
static constexpr const char* ClassName = "Node";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
const long& GetOSMId() { return m_OSMId; }; 
void SetOSMId(long value) { m_OSMId = value; }; 
glm::vec2& GetLongLat() { return m_LongLat; }; 
void SetLongLat(const glm::vec2& value) { m_LongLat = value; }; 
glm::vec3& GetWorldPosition() { return m_WorldPosition; }; 
void SetWorldPosition(const glm::vec3& value) { m_WorldPosition = value; }; 
const float& GetElevation() { return m_Elevation; }; 
void SetElevation(float value) { m_Elevation = value; }; 
std::vector<Way*>& GetWays() { return m_Ways; }; 
void SetWays(std::vector<Way*> value) { m_Ways = value; }; 
void AddTo_Ways(Way* value) { m_Ways.push_back(value); };
glm::ivec2& GetSectorIndex() { return m_SectorIndex; }; 
void SetSectorIndex(const glm::ivec2& value) { m_SectorIndex = value; }; 
glm::vec2& GetSectorOffset() { return m_SectorOffset; }; 
void SetSectorOffset(const glm::vec2& value) { m_SectorOffset = value; }; 
private:
int m_ObjectID;
std::string m_Name;
long m_OSMId;
glm::vec2 m_LongLat = glm::vec2(0, 0);
glm::vec3 m_WorldPosition = glm::vec3(0, 0, 0);
float m_Elevation = 0.f;
std::vector<Way*> m_Ways;
glm::ivec2 m_SectorIndex = glm::ivec2(0, 0);
glm::vec2 m_SectorOffset = glm::vec2(0, 0);
public:
static const int g_PropertyCount = 9;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_NodePIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_OSMId,
PIDX_LongLat,
PIDX_WorldPosition,
PIDX_Elevation,
PIDX_Ways,
PIDX_SectorIndex,
PIDX_SectorOffset,
};
bool Serialize(bool writing);

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

