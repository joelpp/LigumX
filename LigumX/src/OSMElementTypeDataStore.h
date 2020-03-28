#pragma once

#include "OSMElementTypeData.h"
#include <vector>

#pragma region  FORWARD_DECLARATIONS OSMElementTypeDataStore
#include "LXSystem.h"
class Serializer2;

class OSMElementTypeDataStore;
class OSMElementTypeData;

extern OSMElementTypeDataStore* g_OSMElementTypeDataStore;

#pragma endregion  FORWARD_DECLARATIONS OSMElementTypeDataStore

#pragma region  HEADER OSMElementTypeDataStore
class OSMElementTypeDataStore : public LXObject
{
public:
static const int ClassID = -1486964697;
static const LXType Type = LXType_OSMElementTypeDataStore;
static constexpr const char* ClassName = "OSMElementTypeDataStore";
typedef LXObject super;

std::vector<OSMElementTypeData>& GetData() { return m_Data; }; 
void SetData(std::vector<OSMElementTypeData> value) { m_Data = value; }; 
void AddTo_Data(OSMElementTypeData value) { m_Data.push_back(value); };
private:
std::vector<OSMElementTypeData> m_Data;
public:
static const int g_PropertyCount = 1;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_OSMElementTypeDataStorePIDX
{
PIDX_Data,
};
void Serialize(Serializer2& serializer);
bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual const char* GetTypeName();

#pragma endregion  HEADER OSMElementTypeDataStore

OSMElementTypeDataStore();
OSMElementType GetWayType(Way* way);

};
