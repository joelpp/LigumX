#pragma once

#include "EditorOptions.h"

#pragma region  FORWARD_DECLARATIONS Editor
#include "property.h"

class Editor;
class EditorOptions;

extern Editor* g_Editor;

#pragma endregion  FORWARD_DECLARATIONS Editor

class Editor
{
#pragma region  HEADER Editor
public:
static const int ClassID = 1231601560;
static const LXType Type = LXType_Editor;
static constexpr const char* ClassName = "Editor";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
EditorOptions*& GetOptions() { return m_Options; }; 
void SetOptions(EditorOptions* value) { m_Options = value; }; 
private:
int m_ObjectID;
std::string m_Name;
EditorOptions* m_Options;
public:
static const int g_PropertyCount = 3;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_EditorPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_Options,
};
bool Serialize(bool writing);

#pragma endregion  HEADER Editor

Editor();
Editor(int ObjectID);

};
