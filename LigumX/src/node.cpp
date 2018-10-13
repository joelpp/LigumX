#include "stdafx.h"
#include "node.h"

#pragma region  CLASS_SOURCE Node

#include "Node.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData Node::g_Properties[] = 
{
{ "OSMId", PIDX_OSMId, offsetof(Node, m_OSMId), 0, LXType_Object, sizeof(long), LXType_long, false, LXType_None, 0, 0, 0, 0,}, 
{ "LongLat", PIDX_LongLat, offsetof(Node, m_LongLat), 0, LXType_glmvec2, sizeof(glm::vec2), LXType_glmvec2, false, LXType_None, 0, 0, 0, 0,}, 
{ "WorldPosition", PIDX_WorldPosition, offsetof(Node, m_WorldPosition), 0, LXType_glmvec3, sizeof(glm::vec3), LXType_glmvec3, false, LXType_None, 0, 0, 0, 0,}, 
{ "Elevation", PIDX_Elevation, offsetof(Node, m_Elevation), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, 0, 0, 0, 0,}, 
{ "Ways", PIDX_Ways, offsetof(Node, m_Ways), 0, LXType_stdvector, sizeof(std::vector<Way*>), LXType_stdvector, false, LXType_Way, 0, 0, 0, 0,}, 
{ "SectorIndex", PIDX_SectorIndex, offsetof(Node, m_SectorIndex), 0, LXType_glmivec2, sizeof(glm::ivec2), LXType_glmivec2, false, LXType_None, 0, 0, 0, 0,}, 
{ "SectorOffset", PIDX_SectorOffset, offsetof(Node, m_SectorOffset), 0, LXType_glmvec2, sizeof(glm::vec2), LXType_glmvec2, false, LXType_None, 0, 0, 0, 0,}, 
};
bool Node::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}

#pragma endregion  CLASS_SOURCE Node
Node::Node()
{
    this->elevation = 0;
}

Node::Node(glm::vec2 latLong)
{
    this->elevation = 0;
}

Node::Node(std::string _id, float _longitude, float _latitude)
{
    this->id = _id;
    this->latitude = _latitude;
    this->longitude = _longitude;
    this->elevation = 0;
}

Node::Node(std::string _id, float _longitude, float _latitude, float _height)
{
	this->id = _id;
	this->latitude = _latitude;
	this->longitude = _longitude;
	this->elevation = _height;
}



std::string Node::toString()
{
    char str[200];
    sprintf(str, "Node ID=%s lat=%f lon=%f", this->id.c_str(), this->latitude, this->longitude);
    std::string toReturn = std::string(str);
    for ( auto it = this->tags.begin(); it != this->tags.end(); ++it ){
        char tagstr[100];
        
        sprintf(tagstr, " | k: %s v: %s |", it->first.c_str(), it->second.c_str());
        toReturn.append(std::string(tagstr));
    }
    return toReturn;
}

glm::vec2 Node::getLatLong()
{
    return glm::vec2(longitude, latitude);
}

glm::vec3 Node::getLatLongEle()
{
    return glm::vec3(longitude, latitude, elevation);
}

void Node::AddWay(Way* way)
{
	
	for (Way* currentWay : m_Ways)
	{
		if (way == currentWay)
		{
			return;
		}
	}

	m_Ways.push_back(way);
}
