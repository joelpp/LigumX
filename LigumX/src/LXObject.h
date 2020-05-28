#pragma once

#pragma region  FORWARD_DECLARATIONS LXObject
#include "LXSystem.h"
class Serializer2;

class LXObject;


#pragma endregion  FORWARD_DECLARATIONS LXObject

#pragma region  HEADER LXObject
class LXObject
{
public:
static const int ClassID = 1057246886;
static const LXType Type = LXType_LXObject;
static constexpr const char* ClassName = "LXObject";
virtual LXType GetLXType() { return LXType_LXObject; }
virtual const char* GetLXClassName() { return ClassName; }

int GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
private:
int m_ObjectID = 0;
std::string m_Name;
public:
static const int g_PropertyCount = 2;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_LXObjectPIDX
{
PIDX_ObjectID,
PIDX_Name,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual void Clone(LXObject& other);
virtual const char* GetTypeName();

#pragma endregion  HEADER LXObject

};
