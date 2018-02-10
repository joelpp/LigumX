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
const int& GetIndexInSector() { return m_IndexInSector; }; 
void SetIndexInSector(int value) { m_IndexInSector = value; }; 
private:
int m_ObjectID;
std::string m_Name;
int m_OSMId = 0;
std::vector<Node*> m_Nodes;
OSMElementType m_OSMElementType = OSMElementType_Unknown;
int m_IndexInSector = 0;
public:
static const int g_PropertyCount = 6;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_WayPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_OSMId,
PIDX_Nodes,
PIDX_OSMElementType,
PIDX_IndexInSector,
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

