#pragma once

#include "EditorOptions.h"
#include "glm/glm.hpp"
#include <map>
#include <vector>
#include "Logging.h"
#include "CurlRequest.h"

class Texture;
class SectorData;
class Sector;


#pragma region  FORWARD_DECLARATIONS Editor
#include "property.h"

class Editor;
class EditorOptions;
class SectorTool;
class OSMTool;
class PickingTool;
class TerrainTool;
class Node;

extern Editor* g_Editor;
enum EditorTool
{
EditorTool_None,
EditorTool_TerrainTool,
EditorTool_EntityManipulator,
EditorTool_SectorTool,
EditorTool_OSMTool,
EditorTool_PickingTool,
};

extern const std::string EnumValues_EditorTool[6];
extern const EditorTool Indirection_EditorTool[6];
const int EnumLength_EditorTool = 6;


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
const EditorTool& GetActiveTool() { return m_ActiveTool; }; 
void SetActiveTool(EditorTool value) { m_ActiveTool = value; }; 
const glm::vec4& GetXYZMask() { return m_XYZMask; }; 
void SetXYZMask(glm::vec4 value) { m_XYZMask = value; }; 
void AddTo_XYZMask(glm::vec4 value) { m_XYZMask += value; };
const bool& GetManipulatorDragging() { return m_ManipulatorDragging; }; 
void SetManipulatorDragging(bool value) { m_ManipulatorDragging = value; }; 
const glm::vec3& GetManipulatorStartPosition() { return m_ManipulatorStartPosition; }; 
void SetManipulatorStartPosition(glm::vec3 value) { m_ManipulatorStartPosition = value; }; 
const bool& GetEditingTerrain() { return m_EditingTerrain; }; 
void SetEditingTerrain(bool value) { m_EditingTerrain = value; }; 
const bool& GetTerrainErasureMode() { return m_TerrainErasureMode; }; 
void SetTerrainErasureMode(bool value) { m_TerrainErasureMode = value; }; 
const float& GetTerrainBrushSize() { return m_TerrainBrushSize; }; 
void SetTerrainBrushSize(float value) { m_TerrainBrushSize = value; }; 
void AddTo_TerrainBrushSize(float value) { m_TerrainBrushSize += value; };
SectorTool*& GetSectorTool() { return m_SectorTool; }; 
void SetSectorTool(SectorTool* value) { m_SectorTool = value; }; 
OSMTool*& GetOSMTool() { return m_OSMTool; }; 
void SetOSMTool(OSMTool* value) { m_OSMTool = value; }; 
PickingTool*& GetPickingTool() { return m_PickingTool; }; 
void SetPickingTool(PickingTool* value) { m_PickingTool = value; }; 
TerrainTool*& GetTerrainTool() { return m_TerrainTool; }; 
void SetTerrainTool(TerrainTool* value) { m_TerrainTool = value; }; 
const int& GetPickingBufferSize() { return m_PickingBufferSize; }; 
void SetPickingBufferSize(int value) { m_PickingBufferSize = value; }; 
Node*& GetSelectedNode() { return m_SelectedNode; }; 
void SetSelectedNode(Node* value) { m_SelectedNode = value; }; 
private:
int m_ObjectID;
std::string m_Name;
EditorOptions* m_Options;
EditorTool m_ActiveTool;
glm::vec4 m_XYZMask;
bool m_ManipulatorDragging = false;
glm::vec3 m_ManipulatorStartPosition = glm::vec3(0, 0, 0);
bool m_EditingTerrain = false;
bool m_TerrainErasureMode = false;
float m_TerrainBrushSize = 0.f;
SectorTool* m_SectorTool;
OSMTool* m_OSMTool;
PickingTool* m_PickingTool;
TerrainTool* m_TerrainTool;
int m_PickingBufferSize = 0;
Node* m_SelectedNode;
public:
static const int g_PropertyCount = 16;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_EditorPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_Options,
PIDX_ActiveTool,
PIDX_XYZMask,
PIDX_ManipulatorDragging,
PIDX_ManipulatorStartPosition,
PIDX_EditingTerrain,
PIDX_TerrainErasureMode,
PIDX_TerrainBrushSize,
PIDX_SectorTool,
PIDX_OSMTool,
PIDX_PickingTool,
PIDX_TerrainTool,
PIDX_PickingBufferSize,
PIDX_SelectedNode,
};
bool Serialize(bool writing);

#pragma endregion  HEADER Editor

Editor();
Editor(int ObjectID);

void Initialize();

void RenderPicking();
void ApplyTool();

void UpdateManipulator();
void UpdateTerrainEditor();
void UpdateSectorLoader();
void UpdateOSMTool();

void ProcessScrolling();

void Render();
void RenderTools();

void DisplayAxisGizmo();

void RenderImgui();

template <typename T>
void TrySaveObject(T* object);

template<typename T>
void EndShowPropertyGridObject(T*& object, const char* name);

void EndShowPropertyGridObject(Texture*& object, const char* name);

template<typename T>
void ShowPropertyGridObject(T*& object, const char* name);

template<typename T>
bool ShowPropertyGridPtr(T** object, const char* name);

template <typename T>
bool ShowPropertyGridTemplate(T*& object, const char* name);

template<typename T>
void ShowGenericProperty(T*& object, const ClassPropertyData& propertyData);

bool ShowPropertyTemplate(char*& ptr, const char* name, const LXType& type, float min, float max, bool noneditable);

template <typename T>
void ShowObjectCreator();

template<typename T>
void ShowPropertyGridMacro(T* object, const char* name);

template<typename T>
void SaveObjectFromCreator(T* object);

void ShowAddButton(std::vector<char*>* vectorPtr, const LXType& type);

void ShowVariableAsText(glm::vec3 variable, const char* variableName);
void ShowVariableAsText(glm::vec3* variable, const char* variableName);
void ShowVariableAsText(float variable, const char* variableName);
void ShowVariableAsText(int variable, const char* variableName);
void ShowGUIText(const char* text);
void ShowGUIText(std::string* text);
void ShowGUIText(std::string* text, const char* variableName);
void ShowGUIText(const std::string& text);
void ShowGUIText(const std::string& text, const char* variableName);


void ShowProperty(int* value, const char* name);
void ShowProperty(bool* value, const char* name);
void ShowProperty(bool& value, const char* name);
void ShowProperty(int* value, const char* name, int min, int max);

void ShowProperty(std::vector<bool>::reference value, const char* name);


void ShowProperty(float* value, const char* name, float min, float max);
void ShowProperty(glm::vec4* value, const char* name, float min, float max);
void ShowProperty(glm::vec3* value, const char* name, float min, float max);
void ShowProperty(glm::vec2* value, const char* name, float min, float max);
void ShowProperty(glm::ivec2* value, const char* name, float min, float max);
void ShowProperty(std::string* value, const char* name);

template <typename T>
void ShowProperty(std::map<int, char *>* map, const char* name);

template <typename T>
void ShowProperty(std::map<int, T*>* map, const char* name);

template <typename T>
void ShowProperty(std::unordered_map<int, char *>* map, const char* name);

template <typename T>
void ShowProperty(std::unordered_map<int, T*>* map, const char* name);

bool ShowEditableProperty(int* value, const char* name);
void BackupData();

Texture* m_SplatMapTexture;
std::vector<unsigned char> m_SplatMapData;

void DisplayActiveTool();

private:
	bool m_RenderingMenu;
	bool m_LoadingCurlRequest;

	// this is for r&d
	SectorData* m_SectorData;
	Sector* m_Sector;
};
