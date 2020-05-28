#pragma once

#include <string>

#pragma region  FORWARD_DECLARATIONS OSMAttribute
#include "LXSystem.h"
class Serializer2;

class OSMAttribute;


#pragma endregion  FORWARD_DECLARATIONS OSMAttribute

#pragma region  HEADER OSMAttribute
class OSMAttribute : public LXObject
{
public:
static const int ClassID = 255726074;
static const LXType Type = LXType_OSMAttribute;
static constexpr const char* ClassName = "OSMAttribute";
virtual LXType GetLXType() { return LXType_OSMAttribute; }
virtual const char* GetLXClassName() { return ClassName; }
typedef LXObject super;

const std::string& GetKey() { return m_Key; }; 
void SetKey(std::string value) { m_Key = value; }; 
const std::string& GetValue() { return m_Value; }; 
void SetValue(std::string value) { m_Value = value; }; 
private:
std::string m_Key;
std::string m_Value;
public:
static const int g_PropertyCount = 2;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_OSMAttributePIDX
{
PIDX_Key,
PIDX_Value,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual void Clone(OSMAttribute& other);
virtual const char* GetTypeName();

#pragma endregion  HEADER OSMAttribute

OSMAttribute() { };
OSMAttribute(const std::string& key, const std::string& value);

};
