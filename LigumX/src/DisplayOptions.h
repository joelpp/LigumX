#pragma once

#pragma region  FORWARD_DECLARATIONS DisplayOptions
#include "property.h"

class DisplayOptions;


#pragma endregion  FORWARD_DECLARATIONS DisplayOptions

class DisplayOptions
{
public:
public:
public:
#pragma region  HEADER DisplayOptions
public:
static const int ClassID = 2473594549;
static const LXType Type = LXType_DisplayOptions;
static constexpr const char* ClassName = "DisplayOptions";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
const bool& GetUseSkyLighting() { return m_UseSkyLighting; }; 
void SetUseSkyLighting(bool value) { m_UseSkyLighting = value; }; 
const bool& GetRenderTerrain() { return m_RenderTerrain; }; 
void SetRenderTerrain(bool value) { m_RenderTerrain = value; }; 
const bool& GetDrawSky() { return m_DrawSky; }; 
void SetDrawSky(bool value) { m_DrawSky = value; }; 
const bool& GetWireframeRendering() { return m_WireframeRendering; }; 
void SetWireframeRendering(bool value) { m_WireframeRendering = value; }; 
const bool& GetUseLighting() { return m_UseLighting; }; 
void SetUseLighting(bool value) { m_UseLighting = value; }; 
const bool& GetShowNormals() { return m_ShowNormals; }; 
void SetShowNormals(bool value) { m_ShowNormals = value; }; 
const bool& GetShowSpecular() { return m_ShowSpecular; }; 
void SetShowSpecular(bool value) { m_ShowSpecular = value; }; 
const bool& GetShowDiffuse() { return m_ShowDiffuse; }; 
void SetShowDiffuse(bool value) { m_ShowDiffuse = value; }; 
const bool& GetShowAmbient() { return m_ShowAmbient; }; 
void SetShowAmbient(bool value) { m_ShowAmbient = value; }; 
const bool& GetShowFPS() { return m_ShowFPS; }; 
void SetShowFPS(bool value) { m_ShowFPS = value; }; 
const bool& GetShowDepth() { return m_ShowDepth; }; 
void SetShowDepth(bool value) { m_ShowDepth = value; }; 
const bool& GetLinearizeDepth() { return m_LinearizeDepth; }; 
void SetLinearizeDepth(bool value) { m_LinearizeDepth = value; }; 
const bool& GetBlinnPhongShading() { return m_BlinnPhongShading; }; 
void SetBlinnPhongShading(bool value) { m_BlinnPhongShading = value; }; 
const bool& GetRenderTextureOverlay() { return m_RenderTextureOverlay; }; 
void SetRenderTextureOverlay(bool value) { m_RenderTextureOverlay = value; }; 
const bool& GetRenderOpaque() { return m_RenderOpaque; }; 
void SetRenderOpaque(bool value) { m_RenderOpaque = value; }; 
const bool& GetRenderShadows() { return m_RenderShadows; }; 
void SetRenderShadows(bool value) { m_RenderShadows = value; }; 
const bool& GetOutputGLErrors() { return m_OutputGLErrors; }; 
void SetOutputGLErrors(bool value) { m_OutputGLErrors = value; }; 
const bool& GetRenderGrid() { return m_RenderGrid; }; 
void SetRenderGrid(bool value) { m_RenderGrid = value; }; 
private:
int m_ObjectID;
std::string m_Name;
bool m_UseSkyLighting = false;
bool m_RenderTerrain = false;
bool m_DrawSky = false;
bool m_WireframeRendering = false;
bool m_UseLighting = false;
bool m_ShowNormals = false;
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
bool m_RenderGrid = true;
public:
static const int g_PropertyCount = 20;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_DisplayOptionsPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_UseSkyLighting,
PIDX_RenderTerrain,
PIDX_DrawSky,
PIDX_WireframeRendering,
PIDX_UseLighting,
PIDX_ShowNormals,
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
PIDX_RenderGrid,
};
bool Serialize(bool writing);

#pragma endregion  HEADER DisplayOptions

public:

	DisplayOptions::DisplayOptions();
};
