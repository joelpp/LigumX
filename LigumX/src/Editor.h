#pragma once

#include "EditorOptions.h"
#include "glm/glm.hpp"
#include <map>
#include <vector>
#include "Logging.h"
#include "CurlRequest.h"
#include "SectorTool.h"

class Texture;
class SectorData;
class Sector;


#pragma region  FORWARD_DECLARATIONS Editor
#include "property.h"

class Editor;
class EditorOptions;
class Entity;
class SectorTool;
class Node;

extern Editor* g_Editor;
enum EditorTool
{
EditorTool_None,
EditorTool_TerrainHeight,
EditorTool_TerrainSplatMap,
EditorTool_EntityManipulator,
EditorTool_SectorTool,
EditorTool_NodeTool,
EditorTool_WayTool,
};

extern const std::string EnumValues_EditorTool[7];
extern const EditorTool Indirection_EditorTool[7];
const int EnumLength_EditorTool = 7;


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
void AddToXYZMask(glm::vec4 value) { m_XYZMask += value; };
Entity*& GetPickedEntity() { return m_PickedEntity; }; 
void SetPickedEntity(Entity* value) { m_PickedEntity = value; }; 
const glm::vec3& GetPickedWorldPosition() { return m_PickedWorldPosition; }; 
void SetPickedWorldPosition(glm::vec3 value) { m_PickedWorldPosition = value; }; 
const bool& GetManipulatorDragging() { return m_ManipulatorDragging; }; 
void SetManipulatorDragging(bool value) { m_ManipulatorDragging = value; }; 
const glm::vec3& GetManipulatorStartPosition() { return m_ManipulatorStartPosition; }; 
void SetManipulatorStartPosition(glm::vec3 value) { m_ManipulatorStartPosition = value; }; 
const glm::vec4& GetPickingData() { return m_PickingData; }; 
void SetPickingData(glm::vec4 value) { m_PickingData = value; }; 
const bool& GetEditingTerrain() { return m_EditingTerrain; }; 
void SetEditingTerrain(bool value) { m_EditingTerrain = value; }; 
const bool& GetTerrainErasureMode() { return m_TerrainErasureMode; }; 
void SetTerrainErasureMode(bool value) { m_TerrainErasureMode = value; }; 
const float& GetTerrainBrushSize() { return m_TerrainBrushSize; }; 
void SetTerrainBrushSize(float value) { m_TerrainBrushSize = value; }; 
void AddToTerrainBrushSize(float value) { m_TerrainBrushSize += value; };
SectorTool*& GetSectorTool() { return m_SectorTool; }; 
void SetSectorTool(SectorTool* value) { m_SectorTool = value; }; 
Node*& GetSelectedNode() { return m_SelectedNode; }; 
void SetSelectedNode(Node* value) { m_SelectedNode = value; }; 
const int& GetPickingBufferSize() { return m_PickingBufferSize; }; 
void SetPickingBufferSize(int value) { m_PickingBufferSize = value; }; 
private:
int m_ObjectID;
std::string m_Name;
EditorOptions* m_Options;
EditorTool m_ActiveTool;
glm::vec4 m_XYZMask;
Entity* m_PickedEntity;
glm::vec3 m_PickedWorldPosition = glm::vec3(0, 0, 0);
bool m_ManipulatorDragging = false;
glm::vec3 m_ManipulatorStartPosition = glm::vec3(0, 0, 0);
glm::vec4 m_PickingData;
bool m_EditingTerrain = false;
bool m_TerrainErasureMode = false;
float m_TerrainBrushSize = 0.f;
SectorTool* m_SectorTool;
Node* m_SelectedNode;
int m_PickingBufferSize = 0;
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
PIDX_PickedEntity,
PIDX_PickedWorldPosition,
PIDX_ManipulatorDragging,
PIDX_ManipulatorStartPosition,
PIDX_PickingData,
PIDX_EditingTerrain,
PIDX_TerrainErasureMode,
PIDX_TerrainBrushSize,
PIDX_SectorTool,
PIDX_SelectedNode,
PIDX_PickingBufferSize,
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
void UpdateNodeTool();

void ProcessScrolling();

void Render();

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
