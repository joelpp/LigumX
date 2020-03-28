#pragma once

#pragma region  FORWARD_DECLARATIONS OutputSerializer
#include "LXSystem.h"
class Serializer2;

class OutputSerializer;


#pragma endregion  FORWARD_DECLARATIONS OutputSerializer

#include "Serializer2.h"

#pragma region  HEADER OutputSerializer
class OutputSerializer : public Serializer2
{
public:
static const int ClassID = 724455816;
static const LXType Type = LXType_OutputSerializer;
static constexpr const char* ClassName = "OutputSerializer";
typedef Serializer2 super;

private:
public:
static const int g_PropertyCount = 0;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_OutputSerializerPIDX
{
};
void Serialize(Serializer2& serializer);
bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual const char* GetTypeName();

#pragma endregion  HEADER OutputSerializer


OutputSerializer(LXObject* object, bool writing, const std::string& fileName);

};
