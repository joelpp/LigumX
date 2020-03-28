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
typedef Component super;

private:
public:
static const int g_PropertyCount = 0;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_TestComponentAPIDX
{
};
void Serialize(Serializer2& serializer);
bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual const char* GetTypeName();

#pragma endregion  HEADER TestComponentA



};
