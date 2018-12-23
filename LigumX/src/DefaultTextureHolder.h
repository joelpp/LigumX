#pragma once

#pragma region  FORWARD_DECLARATIONS DefaultTextureHolder
#include "LXSystem.h"

class DefaultTextureHolder;
class Texture;

extern DefaultTextureHolder* g_DefaultTextureHolder;

#pragma endregion  FORWARD_DECLARATIONS DefaultTextureHolder

#pragma region  HEADER DefaultTextureHolder
class DefaultTextureHolder : public LXObject
{
public:
static const int ClassID = 4127650523;
static const LXType Type = LXType_DefaultTextureHolder;
static constexpr const char* ClassName = "DefaultTextureHolder";
typedef LXObject super;

Texture*& GetAsphaltTexture() { return m_AsphaltTexture; }; 
void SetAsphaltTexture(Texture* value) { m_AsphaltTexture = value; }; 
Texture*& GetDirtTexture() { return m_DirtTexture; }; 
void SetDirtTexture(Texture* value) { m_DirtTexture = value; }; 
Texture*& GetGrassTexture() { return m_GrassTexture; }; 
void SetGrassTexture(Texture* value) { m_GrassTexture = value; }; 
Texture*& GetRockTexture() { return m_RockTexture; }; 
void SetRockTexture(Texture* value) { m_RockTexture = value; }; 
Texture*& GetWoodTexture() { return m_WoodTexture; }; 
void SetWoodTexture(Texture* value) { m_WoodTexture = value; }; 
Texture*& GetSandTexture() { return m_SandTexture; }; 
void SetSandTexture(Texture* value) { m_SandTexture = value; }; 
Texture*& GetWaterTexture() { return m_WaterTexture; }; 
void SetWaterTexture(Texture* value) { m_WaterTexture = value; }; 
private:
Texture* m_AsphaltTexture = nullptr;
Texture* m_DirtTexture = nullptr;
Texture* m_GrassTexture = nullptr;
Texture* m_RockTexture = nullptr;
Texture* m_WoodTexture = nullptr;
Texture* m_SandTexture = nullptr;
Texture* m_WaterTexture = nullptr;
public:
static const int g_PropertyCount = 7;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_DefaultTextureHolderPIDX
{
PIDX_AsphaltTexture,
PIDX_DirtTexture,
PIDX_GrassTexture,
PIDX_RockTexture,
PIDX_WoodTexture,
PIDX_SandTexture,
PIDX_WaterTexture,
};
bool Serialize(bool writing);
bool ShowPropertyGrid();

#pragma endregion  HEADER DefaultTextureHolder

public:

DefaultTextureHolder();

};
