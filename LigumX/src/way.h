#pragma once

#include "OSMElement.h"

#pragma region  FORWARD_DECLARATIONS Way
#include "LXSystem.h"

class Way;
class Node;


#pragma endregion  FORWARD_DECLARATIONS Way
class LineSegment;
class OSMElement;
class Node;
typedef std::pair<int, int> WayPair;


#pragma region  HEADER Way
class Way : public OSMElement
{
public:
static const int ClassID = 2137978372;
static const LXType Type = LXType_Way;
static constexpr const char* ClassName = "Way";
typedef OSMElement super;

int GetOSMId() { return m_OSMId; }; 
void SetOSMId(int value) { m_OSMId = value; }; 
std::vector<Node*>& GetNodes() { return m_Nodes; }; 
void SetNodes(std::vector<Node*> value) { m_Nodes = value; }; 
void AddTo_Nodes(Node* value) { m_Nodes.push_back(value); };
const OSMElementType& GetOSMElementType() { return m_OSMElementType; }; 
void SetOSMElementType(OSMElementType value) { m_OSMElementType = value; }; 
int GetIndexInSector() { return m_IndexInSector; }; 
void SetIndexInSector(int value) { m_IndexInSector = value; }; 
bool GetFilledIn() { return m_FilledIn; }; 
void SetFilledIn(bool value) { m_FilledIn = value; }; 
const std::string& GetAllTags() { return m_AllTags; }; 
void SetAllTags(std::string value) { m_AllTags = value; }; 
void AddTo_AllTags(std::string value) { m_AllTags += value; };
private:
int m_OSMId = 0;
std::vector<Node*> m_Nodes;
OSMElementType m_OSMElementType = OSMElementType_Unknown;
int m_IndexInSector = 0;
bool m_FilledIn = false;
std::string m_AllTags;
public:
static const int g_PropertyCount = 6;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_WayPIDX
{
PIDX_OSMId,
PIDX_Nodes,
PIDX_OSMElementType,
PIDX_IndexInSector,
PIDX_FilledIn,
PIDX_AllTags,
};
bool Serialize(bool writing);
bool ShowPropertyGrid();

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

	bool IsLandUse() { return m_OSMElementType >= OSMElementType_LanduseRetail && m_OSMElementType <= OSMElementType_LanduseIndustrial; }
	bool IsPark() { return m_OSMElementType == OSMElementType_LeisurePark; }
	bool IsRoad() { return m_OSMElementType >= OSMElementType_HighwayTrunk && m_OSMElementType <= OSMElementType_HighwayUnclassified; }
	bool IsWater() { return m_OSMElementType == OSMElementType_NaturalWater; }
	bool IsWood() { return m_OSMElementType == OSMElementType_NaturalWood; }
	bool IsBareRock() { return m_OSMElementType == OSMElementType_NaturalBareRock; }
	bool IsLandUseRetail() { return m_OSMElementType == OSMElementType_LanduseRetail; }
	bool IsLandUseIndustrial() { return m_OSMElementType == OSMElementType_LanduseIndustrial; }
};

