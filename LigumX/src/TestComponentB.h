#pragma once

#include "Component.h"


#pragma region  FORWARD_DECLARATIONS TestComponentB
#include "LXSystem.h"
class Serializer2;

class TestComponentB;


#pragma endregion  FORWARD_DECLARATIONS TestComponentB
#pragma region  HEADER TestComponentB
class TestComponentB : public Component
{
public:
static const int ClassID = -762015354;
static const LXType Type = LXType_TestComponentB;
static constexpr const char* ClassName = "TestComponentB";
virtual LXType GetLXType() { return LXType_TestComponentB; }
virtual const char* GetLXClassName() { return ClassName; }
typedef Component super;

bool GetBValueLol() { return m_BValueLol; }; 
void SetBValueLol(bool value) { m_BValueLol = value; }; 
private:
bool m_BValueLol = true;
public:
static const int g_PropertyCount = 1;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_TestComponentBPIDX
{
PIDX_BValueLol,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual void Clone(TestComponentB& other);
virtual const char* GetTypeName();

#pragma endregion  HEADER TestComponentB



};
