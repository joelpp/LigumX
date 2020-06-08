#pragma once

#pragma region  FORWARD_DECLARATIONS GFXUniformDescription
#include "LXSystem.h"
class Serializer2;

class GFXUniformDescription;


#pragma endregion  FORWARD_DECLARATIONS GFXUniformDescription
#pragma region  HEADER GFXUniformDescription
class GFXUniformDescription : public LXObject
{
public:
static const int ClassID = 1462035770;
static const LXType Type = LXType_GFXUniformDescription;
static constexpr const char* ClassName = "GFXUniformDescription";
virtual LXType GetLXType() { return LXType_GFXUniformDescription; }
virtual const char* GetLXClassName() { return ClassName; }
typedef LXObject super;

int GetType() { return m_Type; }; 
void SetType(int value) { m_Type = value; }; 
const std::string& GetUniformName() { return m_UniformName; }; 
void SetUniformName(std::string value) { m_UniformName = value; }; 
int GetLocation() { return m_Location; }; 
void SetLocation(int value) { m_Location = value; }; 
private:
int m_Type = -1;
std::string m_UniformName;
int m_Location = -1;
public:
static const int g_PropertyCount = 3;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_GFXUniformDescriptionPIDX
{
PIDX_Type,
PIDX_UniformName,
PIDX_Location,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual void Clone(LXObject* otherObj);
virtual const char* GetTypeName();

#pragma endregion  HEADER GFXUniformDescription

GFXUniformDescription() { lxAssert0(); }
GFXUniformDescription(const char* name, LXType type)
	: m_UniformName(name)
	, m_Type((int)type)
{
}

};
