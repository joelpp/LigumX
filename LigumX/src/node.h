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
const int& GetOSMId() { return m_OSMId; }; 
void SetOSMId(int value) { m_OSMId = value; }; 
const glm::vec2& GetLongLat() { return m_LongLat; }; 
void SetLongLat(glm::vec2 value) { m_LongLat = value; }; 
const glm::vec3& GetWorldPosition() { return m_WorldPosition; }; 
void SetWorldPosition(glm::vec3 value) { m_WorldPosition = value; }; 
const float& GetElevation() { return m_Elevation; }; 
void SetElevation(float value) { m_Elevation = value; }; 
std::vector<Way*>& GetWays() { return m_Ways; }; 
void SetWays(std::vector<Way*> value) { m_Ways = value; }; 
void AddTo_Ways(Way* value) { m_Ways.push_back(value); };
private:
int m_ObjectID;
std::string m_Name;
int m_OSMId = 0;
glm::vec2 m_LongLat = glm::vec2(0, 0);
glm::vec3 m_WorldPosition = glm::vec3(0, 0, 0);
float m_Elevation = 0.f;
std::vector<Way*> m_Ways;
public:
static const int g_PropertyCount = 7;
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
    std::string toString();
    glm::vec2 getLatLong();
    glm::vec3 getLatLongEle();
    bool operator == (Node& n) const 
	{
       return latitude==n.latitude && longitude==n.longitude;
    }

	void AddWay(Way* way);
};

