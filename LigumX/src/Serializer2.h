#pragma once

#pragma region  FORWARD_DECLARATIONS Serializer2
#include "LXSystem.h"
class Serializer2;

class Serializer2;
class LXObject;


#pragma endregion  FORWARD_DECLARATIONS Serializer2
#pragma region  HEADER Serializer2
class Serializer2 : public LXObject
{
public:
static const int ClassID = 2318800129;
static const LXType Type = LXType_Serializer2;
static constexpr const char* ClassName = "Serializer2";
typedef LXObject super;

bool GetWriting() { return m_Writing; }; 
void SetWriting(bool value) { m_Writing = value; }; 
bool GetIsValid() { return m_IsValid; }; 
void SetIsValid(bool value) { m_IsValid = value; }; 
const std::string& GetFilename() { return m_Filename; }; 
void SetFilename(std::string value) { m_Filename = value; }; 
LXObject*& GetObject() { return m_Object; }; 
void SetObject(LXObject* value) { m_Object = value; }; 
private:
bool m_Writing = false;
bool m_IsValid = false;
std::string m_Filename;
LXObject* m_Object = nullptr;
public:
static const int g_PropertyCount = 4;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_Serializer2PIDX
{
PIDX_Writing,
PIDX_IsValid,
PIDX_Filename,
PIDX_Object,
};
bool Serialize(Serializer2& serializer);
bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual const char* GetTypeName();

#pragma endregion  HEADER Serializer2

public:
	Serializer2(LXObject* object, bool writing, const std::string& fileName);
	static Serializer2 CreateSerializer(LXObject* object, bool writing);

	void Close();

private:

std::fstream m_FileStream;

};
