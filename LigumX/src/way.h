#pragma once
#include "OSMElement.h"
#include <vector>
#include "glm/glm.hpp"
#include <string>
#include <iostream>

#pragma region  FORWARD_DECLARATIONS Way
#include "property.h"

class Way;
class Node;

enum OSMElementType
{
OSMElementType_HighwayTrunk,
OSMElementType_HighwayPrimary,
OSMElementType_HighwaySecondary,
OSMElementType_HighwayTertiary,
OSMElementType_HighwayResidential,
OSMElementType_HighwayService,
OSMElementType_HighwayUnclassified,
OSMElementType_Sidewalk,
OSMElementType_Contour,
OSMElementType_Building_Unmarked,
OSMElementType_Building_School,
OSMElementType_Building_Addressinterpolation,
OSMElementType_Boundary,
OSMElementType_LeisurePark,
OSMElementType_NaturalWood,
OSMElementType_NaturalWater,
OSMElementType_Landuse,
OSMElementType_RailwaySubway,
OSMElementType_AddressInterpolation,
OSMElementType_NotImplemented,
OSMElementType_Unknown,
OSMElementType_Count,
};

extern const std::string EnumValues_OSMElementType[22];
extern const OSMElementType Indirection_OSMElementType[22];
const int EnumLength_OSMElementType = 22;


#pragma endregion  FORWARD_DECLARATIONS Way
class LineSegment;
class OSMElement;
class Node;
typedef std::pair<int, int> WayPair;

class Way : public OSMElement
{
#pragma region  HEADER Way
public:
static const int ClassID = 2137978372;
static const LXType Type = LXType_Way;
static constexpr const char* ClassName = "Way";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
const int& GetOSMId() { return m_OSMId; }; 
void SetOSMId(int value) { m_OSMId = value; }; 
std::vector<Node*>& GetNodes() { return m_Nodes; }; 
void SetNodes(std::vector<Node*> value) { m_Nodes = value; }; 
void AddTo_Nodes(Node* value) { m_Nodes.push_back(value); };
const OSMElementType& GetOSMElementType() { return m_OSMElementType; }; 
void SetOSMElementType(OSMElementType value) { m_OSMElementType = value; }; 
private:
int m_ObjectID;
std::string m_Name;
int m_OSMId = 0;
std::vector<Node*> m_Nodes;
OSMElementType m_OSMElementType = OSMElementType_Unknown;
public:
static const int g_PropertyCount = 5;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_WayPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_OSMId,
PIDX_Nodes,
PIDX_OSMElementType,
};
bool Serialize(bool writing);

#pragma endregion  HEADER Way

public:
    Way();
    Way(const std::string& id);
    Way(std::string id, LineSegment ls, int type);

	void AddNode(Node* node);

    std::string toString();

    bool selectable;
    bool hasPointInside(glm::vec2 xy);
    bool loops();

    static WayPair findCommon(const std::vector<Way*> firstNodeWays, const std::vector<Way*> secondNodeWays);
};

