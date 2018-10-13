#pragma once

#pragma region  FORWARD_DECLARATIONS OSMElementTypeData
#include "property.h"
#include "LXObject.h"

class OSMElementTypeData;
class OSMAttribute;


#pragma endregion  FORWARD_DECLARATIONS OSMElementTypeData

#pragma region  HEADER OSMElementTypeData
class OSMElementTypeData : public LXObject
{
public:
static const int ClassID = 3454283446;
static const LXType Type = LXType_OSMElementTypeData;
static constexpr const char* ClassName = "OSMElementTypeData";

std::vector<OSMAttribute*>& GetAttributes() { return m_Attributes; }; 
void SetAttributes(std::vector<OSMAttribute*> value) { m_Attributes = value; }; 
void AddTo_Attributes(OSMAttribute* value) { m_Attributes.push_back(value); };
private:
std::vector<OSMAttribute*> m_Attributes;
public:
static const int g_PropertyCount = 1;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_OSMElementTypeDataPIDX
{
PIDX_Attributes,
};
bool Serialize(bool writing);

#pragma endregion  HEADER OSMElementTypeData

};
