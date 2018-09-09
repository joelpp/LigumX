#pragma once

#pragma region  FORWARD_DECLARATIONS RenderingStats
#include "property.h"

class RenderingStats;


#pragma endregion  FORWARD_DECLARATIONS RenderingStats

class RenderingStats
{
#pragma region  HEADER RenderingStats
public:
static const int ClassID = 2949050158;
static const LXType Type = LXType_RenderingStats;
static constexpr const char* ClassName = "RenderingStats";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
const int& GetNumVisibleEntities() { return m_NumVisibleEntities; }; 
void SetNumVisibleEntities(int value) { m_NumVisibleEntities = value; }; 
void AddTo_NumVisibleEntities(int value) { m_NumVisibleEntities += value; };
const int& GetNumVisibleSectors() { return m_NumVisibleSectors; }; 
void SetNumVisibleSectors(int value) { m_NumVisibleSectors = value; }; 
void AddTo_NumVisibleSectors(int value) { m_NumVisibleSectors += value; };
const int& GetNumDebugModels() { return m_NumDebugModels; }; 
void SetNumDebugModels(int value) { m_NumDebugModels = value; }; 
void AddTo_NumDebugModels(int value) { m_NumDebugModels += value; };
const int& GetNum2DMessages() { return m_Num2DMessages; }; 
void SetNum2DMessages(int value) { m_Num2DMessages = value; }; 
void AddTo_Num2DMessages(int value) { m_Num2DMessages += value; };
const float& Getfps() { return m_fps; }; 
void Setfps(float value) { m_fps = value; }; 
private:
int m_ObjectID;
std::string m_Name;
int m_NumVisibleEntities = 0;
int m_NumVisibleSectors = 0;
int m_NumDebugModels = 0;
int m_Num2DMessages = 0;
float m_fps = 0.f;
public:
static const int g_PropertyCount = 7;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_RenderingStatsPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_NumVisibleEntities,
PIDX_NumVisibleSectors,
PIDX_NumDebugModels,
PIDX_Num2DMessages,
PIDX_fps,
};
bool Serialize(bool writing);

#pragma endregion  HEADER RenderingStats

};