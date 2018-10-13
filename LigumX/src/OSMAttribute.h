#pragma once

#pragma region  FORWARD_DECLARATIONS OSMAttribute
#include "property.h"
#include "LXObject.h"

class OSMAttribute;


#pragma endregion  FORWARD_DECLARATIONS OSMAttribute

#pragma region  HEADER OSMAttribute
class OSMAttribute : public LXObject
{
public:
static const int ClassID = 3768964154;
static const LXType Type = LXType_OSMAttribute;
static constexpr const char* ClassName = "OSMAttribute";

const std::string& Getkey() { return m_key; }; 
void Setkey(std::string value) { m_key = value; }; 
const std::string& Getvalue() { return m_value; }; 
void Setvalue(std::string value) { m_value = value; }; 
private:
std::string m_key;
std::string m_value;
public:
static const int g_PropertyCount = 2;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_OSMAttributePIDX
{
PIDX_key,
PIDX_value,
};
bool Serialize(bool writing);

#pragma endregion  HEADER OSMAttribute

};
