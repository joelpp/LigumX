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
class PickingTool;
class OSMTool;
class SectorTool;
class TerrainTool;


#pragma region  FORWARD_DECLARATIONS Editor
#include "LXSystem.h"
class Serializer2;

class Editor;
class EditorOptions;
class EditorTool;
class Node;

extern Editor* g_Editor;
enum EEditorTool
{
EEditorTool_TerrainTool,
EEditorTool_EntityManipulator,
EEditorTool_SectorTool,
EEditorTool_OSMTool,
EEditorTool_PickingTool,
EEditorTool_None,
};

extern const std::string EnumValues_EEditorTool[6];
extern const EEditorTool Indirection_EEditorTool[6];
const int EnumLength_EEditorTool = 6;


#pragma endregion  FORWARD_DECLARATIONS Editor

#pragma region  HEADER Editor
class Editor : public LXObject
{
public:
static const int ClassID = 1231601560;
static const LXType Type = LXType_Editor;
static constexpr const char* ClassName = "Editor";
typedef LXObject super;

EditorOptions*& GetOptions() { return m_Options; }; 
void SetOptions(EditorOptions* value) { m_Options = value; }; 
const EEditorTool& GetActiveTool() { return m_ActiveTool; }; 
void SetActiveTool(EEditorTool value) { m_ActiveTool = value; }; 
glm::vec4& GetXYZMask() { return m_XYZMask; }; 
void SetXYZMask(const glm::vec4& value) { SetXYZMask_Callback(value); }; 
void SetXYZMask_Callback(const glm::vec4& value);
void AddTo_XYZMask(glm::vec4 value) { m_XYZMask += value; };
bool GetManipulatorDragging() { return m_ManipulatorDragging; }; 
void SetManipulatorDragging(bool value) { m_ManipulatorDragging = value; }; 
glm::vec3& GetManipulatorStartPosition() { return m_ManipulatorStartPosition; }; 
void SetManipulatorStartPosition(const glm::vec3& value) { m_ManipulatorStartPosition = value; }; 
bool GetEditingTerrain() { return m_EditingTerrain; }; 
void SetEditingTerrain(bool value) { m_EditingTerrain = value; }; 
std::vector<EditorTool*>& GetTools() { return m_Tools; }; 
void SetTools(std::vector<EditorTool*> value) { m_Tools = value; }; 
void AddTo_Tools(EditorTool* value) { m_Tools.push_back(value); };
int GetPickingBufferSize() { return m_PickingBufferSize; }; 
void SetPickingBufferSize(int value) { m_PickingBufferSize = value; }; 
Node*& GetSelectedNode() { return m_SelectedNode; }; 
void SetSelectedNode(Node* value) { m_SelectedNode = value; }; 
private:
EditorOptions* m_Options = nullptr;
EEditorTool m_ActiveTool;
glm::vec4 m_XYZMask;
bool m_ManipulatorDragging = false;
glm::vec3 m_ManipulatorStartPosition = glm::vec3(0, 0, 0);
bool m_EditingTerrain = false;
std::vector<EditorTool*> m_Tools;
int m_PickingBufferSize = 0;
Node* m_SelectedNode = nullptr;
public:
static const int g_PropertyCount = 9;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_EditorPIDX
{
PIDX_Options,
PIDX_ActiveTool,
PIDX_XYZMask,
PIDX_ManipulatorDragging,
PIDX_ManipulatorStartPosition,
PIDX_EditingTerrain,
PIDX_Tools,
PIDX_PickingBufferSize,
PIDX_SelectedNode,
};
void Serialize(Serializer2& serializer);
bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual const char* GetTypeName();

#pragma endregion  HEADER Editor

Editor();
Editor(int ObjectID);

void Initialize();

void RenderPicking();
void ApplyTool();

void ResetWorld();

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

template <typename T>
bool ShowPropertyGridForObject(T*& object, const char* name);

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

void HandleInputEvent(int button, int action, int mods);

void ShowVariableAsText(glm::vec3 variable, const char* variableName);
void ShowVariableAsText(glm::vec3* variable, const char* variableName);
void ShowVariableAsText(float variable, const char* variableName);
void ShowVariableAsText(int variable, const char* variableName);
void ShowGUIText(const char* text);
void ShowGUIText(std::string* text);
void ShowGUIText(std::string* text, const char* variableName);
void ShowGUIText(const std::string& text);
void ShowGUIText(const std::string& text, const char* variableName);


bool ShowProperty(int* value, const char* name);
bool ShowProperty(bool* value, const char* name);
bool ShowProperty(bool& value, const char* name);
bool ShowProperty(int* value, const char* name, int min, int max);

bool ShowProperty(std::vector<bool>::reference value, const char* name);


bool ShowProperty(float* value, const char* name, float min, float max);
bool ShowProperty(glm::vec4* value, const char* name, float min, float max);
bool ShowProperty(glm::vec3* value, const char* name, float min, float max);
bool ShowProperty(glm::vec2* value, const char* name, float min, float max);
bool ShowProperty(glm::ivec2* value, const char* name, float min, float max);
bool ShowProperty(std::string* value, const char* name);

template <typename T, typename U>
bool ShowProperty(std::map<U, char *>* map, const char* name);

template <typename T, typename U>
bool ShowProperty(std::map<U, T*>* map, const char* name);

template <typename T, typename U>
bool ShowProperty(std::unordered_map<U, char *>* map, const char* name);

template <typename T, typename U>
bool ShowProperty(std::unordered_map<U, T*>* map, const char* name);

bool ShowEditableProperty(int* value, const char* name);
void BackupData();

void DisplayActiveTool();

EditorTool* GetTool() { return m_Tools[m_ActiveTool]; }

#define DEFINE_TOOL_GETTER(name) name* Get##name() { return (##name *) m_Tools[EEditorTool_##name]; }

DEFINE_TOOL_GETTER(PickingTool);
DEFINE_TOOL_GETTER(TerrainTool);
//PickingTool* GetPickingTool() { return (PickingTool*)m_Tools[EEditorTool_PickingTool]; }
SectorTool* GetSectorTool() { return (SectorTool*)m_Tools[EEditorTool_SectorTool]; }
OSMTool* GetOSMTool() { return (OSMTool*) m_Tools[EEditorTool_OSMTool]; }

private:
	std::vector<int> m_ToolDisplayToggles;

	bool m_RenderingMenu;
	bool m_LoadingCurlRequest;

	// this is for r&d
	SectorData* m_SectorData;
	Sector* m_Sector;

};
