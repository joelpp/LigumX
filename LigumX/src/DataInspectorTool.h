#pragma once

#pragma region  FORWARD_DECLARATIONS DataInspectorTool
#include "LXSystem.h"
class Serializer2;

class DataInspectorTool;


#pragma endregion  FORWARD_DECLARATIONS DataInspectorTool

#include "EditorTool.h"

#pragma region  HEADER DataInspectorTool
class DataInspectorTool : public EditorTool
{
public:
static const int ClassID = -1977875512;
static const LXType Type = LXType_DataInspectorTool;
static constexpr const char* ClassName = "DataInspectorTool";
virtual LXType GetLXType() { return LXType_DataInspectorTool; }
virtual const char* GetLXClassName() { return ClassName; }
typedef EditorTool super;

private:
public:
static const int g_PropertyCount = 0;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_DataInspectorToolPIDX
{
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual void Clone(DataInspectorTool& other);
virtual const char* GetTypeName();

#pragma endregion  HEADER DataInspectorTool

virtual void DrawImguiWindow();


};
