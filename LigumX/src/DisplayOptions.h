#pragma once

#pragma region  FORWARD_DECLARATIONS DisplayOptions
#include "LXSystem.h"
class Serializer2;

class DisplayOptions;

enum DisplayMode
{
DisplayMode_Final,
DisplayMode_Depth,
DisplayMode_UV,
DisplayMode_Diffuse,
DisplayMode_Normals,
};

extern const std::string EnumValues_DisplayMode[5];
extern const DisplayMode Indirection_DisplayMode[5];
const int EnumLength_DisplayMode = 5;


#pragma endregion  FORWARD_DECLARATIONS DisplayOptions

#pragma region  HEADER DisplayOptions
class DisplayOptions : public LXObject
{
public:
static const int ClassID = -1821372747;
static const LXType Type = LXType_DisplayOptions;
static constexpr const char* ClassName = "DisplayOptions";
typedef LXObject super;

bool GetUseSkyLighting() { return m_UseSkyLighting; }; 
void SetUseSkyLighting(bool value) { m_UseSkyLighting = value; }; 
bool GetRenderTerrain() { return m_RenderTerrain; }; 
void SetRenderTerrain(bool value) { m_RenderTerrain = value; }; 
bool GetDrawSky() { return m_DrawSky; }; 
void SetDrawSky(bool value) { m_DrawSky = value; }; 
bool GetWireframeRendering() { return m_WireframeRendering; }; 
void SetWireframeRendering(bool value) { m_WireframeRendering = value; }; 
bool GetUseLighting() { return m_UseLighting; }; 
void SetUseLighting(bool value) { m_UseLighting = value; }; 
bool GetShowNormals() { return m_ShowNormals; }; 
void SetShowNormals(bool value) { m_ShowNormals = value; }; 
bool GetShowUVs() { return m_ShowUVs; }; 
void SetShowUVs(bool value) { m_ShowUVs = value; }; 
bool GetShowSpecular() { return m_ShowSpecular; }; 
void SetShowSpecular(bool value) { m_ShowSpecular = value; }; 
bool GetShowDiffuse() { return m_ShowDiffuse; }; 
void SetShowDiffuse(bool value) { m_ShowDiffuse = value; }; 
bool GetShowAmbient() { return m_ShowAmbient; }; 
void SetShowAmbient(bool value) { m_ShowAmbient = value; }; 
bool GetShowFPS() { return m_ShowFPS; }; 
void SetShowFPS(bool value) { m_ShowFPS = value; }; 
bool GetShowDepth() { return m_ShowDepth; }; 
void SetShowDepth(bool value) { m_ShowDepth = value; }; 
bool GetLinearizeDepth() { return m_LinearizeDepth; }; 
void SetLinearizeDepth(bool value) { m_LinearizeDepth = value; }; 
bool GetBlinnPhongShading() { return m_BlinnPhongShading; }; 
void SetBlinnPhongShading(bool value) { m_BlinnPhongShading = value; }; 
bool GetRenderTextureOverlay() { return m_RenderTextureOverlay; }; 
void SetRenderTextureOverlay(bool value) { m_RenderTextureOverlay = value; }; 
bool GetRenderOpaque() { return m_RenderOpaque; }; 
void SetRenderOpaque(bool value) { m_RenderOpaque = value; }; 
bool GetRenderShadows() { return m_RenderShadows; }; 
void SetRenderShadows(bool value) { m_RenderShadows = value; }; 
bool GetOutputGLErrors() { return m_OutputGLErrors; }; 
void SetOutputGLErrors(bool value) { m_OutputGLErrors = value; }; 
bool GetDisplayDebugModels() { return m_DisplayDebugModels; }; 
void SetDisplayDebugModels(bool value) { m_DisplayDebugModels = value; }; 
bool GetPickingEnabled() { return m_PickingEnabled; }; 
void SetPickingEnabled(bool value) { m_PickingEnabled = value; }; 
const DisplayMode& GetDisplayMode() { return m_DisplayMode; }; 
void SetDisplayMode(DisplayMode value) { m_DisplayMode = value; }; 
private:
bool m_UseSkyLighting = false;
bool m_RenderTerrain = false;
bool m_DrawSky = false;
bool m_WireframeRendering = false;
bool m_UseLighting = false;
bool m_ShowNormals = false;
bool m_ShowUVs = false;
bool m_ShowSpecular = false;
bool m_ShowDiffuse = false;
bool m_ShowAmbient = false;
bool m_ShowFPS = false;
bool m_ShowDepth = false;
bool m_LinearizeDepth = false;
bool m_BlinnPhongShading = false;
bool m_RenderTextureOverlay = false;
bool m_RenderOpaque = false;
bool m_RenderShadows = false;
bool m_OutputGLErrors = false;
bool m_DisplayDebugModels = false;
bool m_PickingEnabled = true;
DisplayMode m_DisplayMode = DisplayMode_Final;
public:
static const int g_PropertyCount = 21;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_DisplayOptionsPIDX
{
PIDX_UseSkyLighting,
PIDX_RenderTerrain,
PIDX_DrawSky,
PIDX_WireframeRendering,
PIDX_UseLighting,
PIDX_ShowNormals,
PIDX_ShowUVs,
PIDX_ShowSpecular,
PIDX_ShowDiffuse,
PIDX_ShowAmbient,
PIDX_ShowFPS,
PIDX_ShowDepth,
PIDX_LinearizeDepth,
PIDX_BlinnPhongShading,
PIDX_RenderTextureOverlay,
PIDX_RenderOpaque,
PIDX_RenderShadows,
PIDX_OutputGLErrors,
PIDX_DisplayDebugModels,
PIDX_PickingEnabled,
PIDX_DisplayMode,
};
void Serialize(Serializer2& serializer);
bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual const char* GetTypeName();

#pragma endregion  HEADER DisplayOptions

public:

	DisplayOptions::DisplayOptions();
};
