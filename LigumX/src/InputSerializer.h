#pragma once

#pragma region  FORWARD_DECLARATIONS InputSerializer
#include "LXSystem.h"
class Serializer2;

class InputSerializer;


#pragma endregion  FORWARD_DECLARATIONS InputSerializer

#include "Serializer2.h"

#pragma region  HEADER InputSerializer
class InputSerializer : public Serializer2
{
public:
static const int ClassID = -456708097;
static const LXType Type = LXType_InputSerializer;
static constexpr const char* ClassName = "InputSerializer";
typedef Serializer2 super;

private:
public:
static const int g_PropertyCount = 0;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_InputSerializerPIDX
{
};
void Serialize(Serializer2& serializer);
bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual const char* GetTypeName();

#pragma endregion  HEADER InputSerializer

InputSerializer(LXObject* object, bool writing, const std::string& fileName);


};
