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
static const int ClassID = 969288629;
static const LXType Type = LXType_DisplayOptions;
static constexpr const char* ClassName = "DisplayOptions";
virtual LXType GetLXType() { return LXType_DisplayOptions; }
virtual const char* GetLXClassName() { return ClassName; }
typedef LXObject super;

bool GetDeferredRendering() { return m_DeferredRendering; }; 
void SetDeferredRendering(bool value) { m_DeferredRendering = value; }; 
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
bool GetShowDebugIcons() { return m_ShowDebugIcons; }; 
void SetShowDebugIcons(bool value) { m_ShowDebugIcons = value; }; 
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
bool GetDebugShadowCamera() { return m_DebugShadowCamera; }; 
void SetDebugShadowCamera(bool value) { m_DebugShadowCamera = value; }; 
const DisplayMode& GetDisplayMode() { return m_DisplayMode; }; 
void SetDisplayMode(DisplayMode value) { m_DisplayMode = value; }; 
glm::vec2& GetFPSDisplayPosition() { return m_FPSDisplayPosition; }; 
void SetFPSDisplayPosition(const glm::vec2& value) { m_FPSDisplayPosition = value; }; 
float GetFPSDisplayScale() { return m_FPSDisplayScale; }; 
void SetFPSDisplayScale(float value) { m_FPSDisplayScale = value; }; 
glm::vec4& GetDebugVec4() { return m_DebugVec4; }; 
void SetDebugVec4(const glm::vec4& value) { m_DebugVec4 = value; }; 
private:
bool m_DeferredRendering = false;
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
bool m_ShowDebugIcons = false;
bool m_LinearizeDepth = false;
bool m_BlinnPhongShading = false;
bool m_RenderTextureOverlay = false;
bool m_RenderOpaque = false;
bool m_RenderShadows = false;
bool m_OutputGLErrors = false;
bool m_DisplayDebugModels = false;
bool m_PickingEnabled = true;
bool m_DebugShadowCamera = false;
DisplayMode m_DisplayMode = DisplayMode_Final;
glm::vec2 m_FPSDisplayPosition = glm::vec2(0, 0);
float m_FPSDisplayScale = 0.1f;
glm::vec4 m_DebugVec4;
public:
static const int g_PropertyCount = 27;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_DisplayOptionsPIDX
{
PIDX_DeferredRendering,
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
PIDX_ShowDebugIcons,
PIDX_LinearizeDepth,
PIDX_BlinnPhongShading,
PIDX_RenderTextureOverlay,
PIDX_RenderOpaque,
PIDX_RenderShadows,
PIDX_OutputGLErrors,
PIDX_DisplayDebugModels,
PIDX_PickingEnabled,
PIDX_DebugShadowCamera,
PIDX_DisplayMode,
PIDX_FPSDisplayPosition,
PIDX_FPSDisplayScale,
PIDX_DebugVec4,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual void Clone(LXObject* otherObj);
virtual const char* GetTypeName();

#pragma endregion  HEADER DisplayOptions

public:

	DisplayOptions::DisplayOptions();
};
