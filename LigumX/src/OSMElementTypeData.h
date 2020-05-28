#pragma once

#include "OSMAttribute.h"
#include "OSMElement.h"

#include "glm\glm.hpp"

#pragma region  FORWARD_DECLARATIONS OSMElementTypeData
#include "LXSystem.h"
class Serializer2;

class OSMElementTypeData;
class OSMAttribute;


#pragma endregion  FORWARD_DECLARATIONS OSMElementTypeData

#pragma region  HEADER OSMElementTypeData
class OSMElementTypeData : public LXObject
{
public:
static const int ClassID = 2032613078;
static const LXType Type = LXType_OSMElementTypeData;
static constexpr const char* ClassName = "OSMElementTypeData";
virtual const char* GetLXClassName() { return ClassName; }
typedef LXObject super;

const OSMElementType& GetType() { return m_Type; }; 
void SetType(OSMElementType value) { m_Type = value; }; 
bool GetFillIn() { return m_FillIn; }; 
void SetFillIn(bool value) { m_FillIn = value; }; 
glm::vec3& GetDebugColor() { return m_DebugColor; }; 
void SetDebugColor(const glm::vec3& value) { m_DebugColor = value; }; 
std::vector<OSMAttribute*>& GetAttributes() { return m_Attributes; }; 
void SetAttributes(std::vector<OSMAttribute*> value) { m_Attributes = value; }; 
void AddTo_Attributes(OSMAttribute* value) { m_Attributes.push_back(value); };
private:
OSMElementType m_Type;
bool m_FillIn = false;
glm::vec3 m_DebugColor = glm::vec3(0, 0, 0);
std::vector<OSMAttribute*> m_Attributes;
public:
static const int g_PropertyCount = 4;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_OSMElementTypeDataPIDX
{
PIDX_Type,
PIDX_FillIn,
PIDX_DebugColor,
PIDX_Attributes,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual void Clone(OSMElementTypeData& other);
virtual const char* GetTypeName();

#pragma endregion  HEADER OSMElementTypeData

bool GetIsBuilding();

};
