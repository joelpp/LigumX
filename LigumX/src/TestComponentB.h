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
static const int ClassID = -1331125850;
static const LXType Type = LXType_TestComponentB;
static constexpr const char* ClassName = "TestComponentB";
typedef Component super;

private:
public:
static const int g_PropertyCount = 0;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_TestComponentBPIDX
{
};
void Serialize(Serializer2& serializer);
bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual const char* GetTypeName();

#pragma endregion  HEADER TestComponentB



};
