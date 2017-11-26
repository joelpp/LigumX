#pragma once

#include "EditorOptions.h"
#include "glm/glm.hpp"
#include <map>
#include <vector>
#include "imgui_impl_glfw_gl3.h"
#include "Logging.h"

class Texture;

#pragma region  FORWARD_DECLARATIONS Editor
#include "property.h"

class Editor;
class EditorOptions;
class Entity;

extern Editor* g_Editor;

#pragma endregion  FORWARD_DECLARATIONS Editor

class Editor
{
REGISTERCLASS(Editor);

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
const glm::vec2& GetMouseClickPosition() { return m_MouseClickPosition; }; 
void SetMouseClickPosition(glm::vec2 value) { m_MouseClickPosition = value; }; 
const glm::vec2& GetLastMouseClickPosition() { return m_LastMouseClickPosition; }; 
void SetLastMouseClickPosition(glm::vec2 value) { m_LastMouseClickPosition = value; }; 
const glm::vec2& GetLastMousePosition() { return m_LastMousePosition; }; 
void SetLastMousePosition(glm::vec2 value) { m_LastMousePosition = value; }; 
const glm::vec2& GetMousePosition() { return m_MousePosition; }; 
void SetMousePosition(glm::vec2 value) { m_MousePosition = value; }; 
const bool& GetMouseButton1Down() { return m_MouseButton1Down; }; 
void SetMouseButton1Down(bool value) { m_MouseButton1Down = value; }; 
const glm::vec4& GetXYZMask() { return m_XYZMask; }; 
void SetXYZMask(glm::vec4 value) { m_XYZMask = value; }; 
void AddToXYZMask(glm::vec4 value) { m_XYZMask += value; };
Entity*& GetPickedEntity() { return m_PickedEntity; }; 
void SetPickedEntity(Entity* value) { m_PickedEntity = value; }; 
const glm::vec3& GetPickedWorldPosition() { return m_PickedWorldPosition; }; 
void SetPickedWorldPosition(glm::vec3 value) { m_PickedWorldPosition = value; }; 
const bool& GetManipulatorDragging() { return m_ManipulatorDragging; }; 
void SetManipulatorDragging(bool value) { m_ManipulatorDragging = value; }; 
const glm::vec2& GetMouseDragDistance() { return m_MouseDragDistance; }; 
void SetMouseDragDistance(glm::vec2 value) { m_MouseDragDistance = value; }; 
const glm::ivec2& GetPickedTexelOffset() { return m_PickedTexelOffset; }; 
void SetPickedTexelOffset(glm::ivec2 value) { m_PickedTexelOffset = value; }; 
const bool& GetTerrainErasureMode() { return m_TerrainErasureMode; }; 
void SetTerrainErasureMode(bool value) { m_TerrainErasureMode = value; }; 
const float& GetTerrainBrushSize() { return m_TerrainBrushSize; }; 
void SetTerrainBrushSize(float value) { m_TerrainBrushSize = value; }; 
void AddToTerrainBrushSize(float value) { m_TerrainBrushSize += value; };
const int& GetPickingBufferSize() { return m_PickingBufferSize; }; 
void SetPickingBufferSize(int value) { m_PickingBufferSize = value; }; 
private:
int m_ObjectID;
std::string m_Name;
EditorOptions* m_Options;
glm::vec2 m_MouseClickPosition;
glm::vec2 m_LastMouseClickPosition;
glm::vec2 m_LastMousePosition;
glm::vec2 m_MousePosition;
bool m_MouseButton1Down;
glm::vec4 m_XYZMask;
Entity* m_PickedEntity;
glm::vec3 m_PickedWorldPosition;
bool m_ManipulatorDragging;
glm::vec2 m_MouseDragDistance;
glm::ivec2 m_PickedTexelOffset;
bool m_TerrainErasureMode;
float m_TerrainBrushSize;
int m_PickingBufferSize;
public:
static const int g_PropertyCount = 17;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_EditorPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_Options,
PIDX_MouseClickPosition,
PIDX_LastMouseClickPosition,
PIDX_LastMousePosition,
PIDX_MousePosition,
PIDX_MouseButton1Down,
PIDX_XYZMask,
PIDX_PickedEntity,
PIDX_PickedWorldPosition,
PIDX_ManipulatorDragging,
PIDX_MouseDragDistance,
PIDX_PickedTexelOffset,
PIDX_TerrainErasureMode,
PIDX_TerrainBrushSize,
PIDX_PickingBufferSize,
};
bool Serialize(bool writing);

#pragma endregion  HEADER Editor

Editor();
Editor(int ObjectID);

void Initialize();

void RenderPicking();

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

void BeginImGUIWindow(unsigned int xSize, unsigned int ySize, ImGuiWindowFlags flags, bool open, const char* name);
void EndImGUIWindow();

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
void ShowProperty(glm::ivec2* value, const char* name, float min, float max);
void ShowProperty(std::string* value, const char* name);

template <typename T>
void ShowProperty(std::map<int, char *>* map, const char* name);

bool ShowEditableProperty(int* value, const char* name);
void BackupData();

Texture* m_SplatMapTexture;
std::vector<unsigned char> m_SplatMapData;
private:
	bool m_RenderingMenu;
};
