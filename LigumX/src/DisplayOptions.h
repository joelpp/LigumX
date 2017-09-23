#pragma once

#pragma region  FORWARD_DECLARATIONS DisplayOptions
#include "property.h"

#pragma endregion  FORWARD_DECLARATIONS DisplayOptions

class DisplayOptions
{
#pragma region  HEADER DisplayOptions
static const int ClassID = 2473594549;
public:
const bool& GetDrawTerrain() { return m_DrawTerrain; }; 
void SetDrawTerrain(bool value) { m_DrawTerrain = value; };
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
private:
bool m_DrawTerrain;
bool m_DrawSky;
bool m_WireframeRendering;
bool m_UseLighting;
bool m_ShowNormals;
bool m_ShowSpecular;
bool m_ShowDiffuse;
bool m_ShowAmbient;
bool m_ShowFPS;
bool m_ShowDepth;
bool m_LinearizeDepth;
bool m_BlinnPhongShading;
bool m_RenderTextureOverlay;
public:
static const int g_DisplayOptionsPropertyCount = 13;
static const ClassPropertyData g_DisplayOptionsProperties[g_DisplayOptionsPropertyCount];


#pragma endregion  HEADER DisplayOptions

};