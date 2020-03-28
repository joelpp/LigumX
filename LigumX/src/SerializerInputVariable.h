#pragma once

#pragma region  FORWARD_DECLARATIONS SerializerInputVariable
#include "LXSystem.h"
class Serializer2;

class SerializerInputVariable;


#pragma endregion  FORWARD_DECLARATIONS SerializerInputVariable
#pragma region  HEADER SerializerInputVariable
class SerializerInputVariable : public LXObject
{
public:
static const int ClassID = 929535779;
static const LXType Type = LXType_SerializerInputVariable;
static constexpr const char* ClassName = "SerializerInputVariable";
virtual const char* GetLXClassName() { return ClassName; }
typedef LXObject super;

const std::string& GetVariableName() { return m_VariableName; }; 
void SetVariableName(std::string value) { m_VariableName = value; }; 
std::vector<std::string>& GetValues() { return m_Values; }; 
void SetValues(std::vector<std::string> value) { m_Values = value; }; 
void AddTo_Values(std::string value) { m_Values.push_back(value); };
private:
std::string m_VariableName;
std::vector<std::string> m_Values;
public:
static const int g_PropertyCount = 2;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_SerializerInputVariablePIDX
{
PIDX_VariableName,
PIDX_Values,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual const char* GetTypeName();

#pragma endregion  HEADER SerializerInputVariable



};
