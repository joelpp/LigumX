#pragma once

#pragma region  FORWARD_DECLARATIONS Serializer
#include "LXSystem.h"

class Serializer;


#pragma endregion  FORWARD_DECLARATIONS Serializer

class Serializer : public LXObject
{
#pragma region  HEADER Serializer
public:
static const int ClassID = 233765801;
static constexpr const char* ClassName = "Serializer";
typedef LXObject super;

bool Getwriting() { return m_writing; }; 
void Setwriting(bool value) { m_writing = value; }; 
bool GetIsValid() { return m_IsValid; }; 
void SetIsValid(bool value) { m_IsValid = value; }; 
const std::string& GetFilename() { return m_Filename; }; 
void SetFilename(std::string value) { m_Filename = value; }; 
private:
bool m_writing = false;
bool m_IsValid = false;
std::string m_Filename;
public:
static const int g_PropertyCount = 3;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_SerializerPIDX
{
PIDX_writing,
PIDX_IsValid,
PIDX_Filename,
};
bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual const char* GetTypeName();

#pragma endregion  HEADER Serializer

};
