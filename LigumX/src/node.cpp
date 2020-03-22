#include "stdafx.h"

#pragma region  CLASS_SOURCE Node

#include "Node.h"
#include "serializer.h"
#include "Way.h"
const ClassPropertyData Node::g_Properties[] = 
{
{ "OSMId", PIDX_OSMId, offsetof(Node, m_OSMId), 0, LXType_Object, sizeof(lxInt64), LXType_lxInt64, false, LXType_None, false, 0, LX_LIMITS_INT64_MIN, LX_LIMITS_INT64_MAX, 0,}, 
{ "LongLat", PIDX_LongLat, offsetof(Node, m_LongLat), 0, LXType_glmvec2, sizeof(glm::vec2), LXType_glmvec2, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "WorldPosition", PIDX_WorldPosition, offsetof(Node, m_WorldPosition), 0, LXType_glmvec3, sizeof(glm::vec3), LXType_glmvec3, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "Elevation", PIDX_Elevation, offsetof(Node, m_Elevation), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "Ways", PIDX_Ways, offsetof(Node, m_Ways), 0, LXType_stdvector, sizeof(std::vector<Way*>), LXType_stdvector, false, LXType_Way, true, 0, 0, 0, 0,}, 
{ "SectorIndex", PIDX_SectorIndex, offsetof(Node, m_SectorIndex), 0, LXType_glmivec2, sizeof(glm::ivec2), LXType_glmivec2, false, LXType_None, false, 0, (float)LX_LIMITS_INT_MIN, (float)LX_LIMITS_INT_MAX, 0,}, 
{ "SectorOffset", PIDX_SectorOffset, offsetof(Node, m_SectorOffset), 0, LXType_glmvec2, sizeof(glm::vec2), LXType_glmvec2, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "SectorRelativePosition", PIDX_SectorRelativePosition, offsetof(Node, m_SectorRelativePosition), 0, LXType_glmvec2, sizeof(glm::vec2), LXType_glmvec2, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "QuantizedEarthPosition", PIDX_QuantizedEarthPosition, offsetof(Node, m_QuantizedEarthPosition), 0, LXType_glmivec2, sizeof(glm::ivec2), LXType_glmivec2, false, LXType_None, false, 0, (float)LX_LIMITS_INT_MIN, (float)LX_LIMITS_INT_MAX, 0,}, 
{ "QuantizedSectorPosition", PIDX_QuantizedSectorPosition, offsetof(Node, m_QuantizedSectorPosition), 0, LXType_glmivec2, sizeof(glm::ivec2), LXType_glmivec2, false, LXType_None, false, 0, (float)LX_LIMITS_INT_MIN, (float)LX_LIMITS_INT_MAX, 0,}, 
{ "HighPrecisionEarthCoordinates", PIDX_HighPrecisionEarthCoordinates, offsetof(Node, m_HighPrecisionEarthCoordinates), 0, LXType_glmhighp_ivec2, sizeof(glm::highp_ivec2), LXType_glmhighp_ivec2, false, LXType_None, false, 0, LX_LIMITS_INT64_MIN, LX_LIMITS_INT64_MAX, 0,}, 
};
void Node::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeInt64(g_Properties[PIDX_OSMId], m_OSMId);
	serializer.SerializeVec2(g_Properties[PIDX_LongLat], m_LongLat);
	serializer.SerializeVec3(g_Properties[PIDX_WorldPosition], m_WorldPosition);
	serializer.SerializeFloat(g_Properties[PIDX_Elevation], m_Elevation);
	serializer.SerializeVector(g_Properties[PIDX_Ways], m_Ways);
	serializer.SerializeIVec2(g_Properties[PIDX_SectorIndex], m_SectorIndex);
	serializer.SerializeVec2(g_Properties[PIDX_SectorOffset], m_SectorOffset);
	serializer.SerializeVec2(g_Properties[PIDX_SectorRelativePosition], m_SectorRelativePosition);
	serializer.SerializeIVec2(g_Properties[PIDX_QuantizedEarthPosition], m_QuantizedEarthPosition);
	serializer.SerializeIVec2(g_Properties[PIDX_QuantizedSectorPosition], m_QuantizedSectorPosition);
	serializer.SerializeHighp_IVec2(g_Properties[PIDX_HighPrecisionEarthCoordinates], m_HighPrecisionEarthCoordinates);
}
bool Node::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool Node::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_OSMId], &m_OSMId , LX_LIMITS_INT64_MIN, LX_LIMITS_INT64_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_LongLat], &m_LongLat , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_WorldPosition], &m_WorldPosition , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_Elevation], &m_Elevation , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_Ways], m_Ways  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_SectorIndex], &m_SectorIndex , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_SectorOffset], &m_SectorOffset , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_SectorRelativePosition], &m_SectorRelativePosition , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_QuantizedEarthPosition], &m_QuantizedEarthPosition , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_QuantizedSectorPosition], &m_QuantizedSectorPosition , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_HighPrecisionEarthCoordinates], &m_HighPrecisionEarthCoordinates , LX_LIMITS_INT64_MIN, LX_LIMITS_INT64_MAX );
	return true;
}
const char* Node::GetTypeName()
{
	return ClassName;
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
