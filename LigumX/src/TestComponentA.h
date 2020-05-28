#pragma once

#include "Component.h"

#pragma region  FORWARD_DECLARATIONS TestComponentA
#include "LXSystem.h"
class Serializer2;

class TestComponentA;


#pragma endregion  FORWARD_DECLARATIONS TestComponentA
#pragma region  HEADER TestComponentA
class TestComponentA : public Component
{
public:
static const int ClassID = -762015789;
static const LXType Type = LXType_TestComponentA;
static constexpr const char* ClassName = "TestComponentA";
virtual LXType GetLXType() { return LXType_TestComponentA; }
virtual const char* GetLXClassName() { return ClassName; }
typedef Component super;

bool GetAValue() { return m_AValue; }; 
void SetAValue(bool value) { m_AValue = value; }; 
private:
bool m_AValue = false;
public:
static const int g_PropertyCount = 1;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_TestComponentAPIDX
{
PIDX_AValue,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual void Clone(TestComponentA& other);
virtual const char* GetTypeName();

#pragma endregion  HEADER TestComponentA



};
