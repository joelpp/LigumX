#pragma once

#pragma region  FORWARD_DECLARATIONS EngineStats
#include "property.h"

class EngineStats;

extern EngineStats* g_EngineStats;

#pragma endregion  FORWARD_DECLARATIONS EngineStats

class EngineStats
{
#pragma region  HEADER EngineStats
public:
static const int ClassID = 2882477190;
static const LXType Type = LXType_EngineStats;
static constexpr const char* ClassName = "EngineStats";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
const int& GetNumObjectMapHits() { return m_NumObjectMapHits; }; 
void SetNumObjectMapHits(int value) { m_NumObjectMapHits = value; }; 
void AddToNumObjectMapHits(int value) { m_NumObjectMapHits += value; };
const int& GetNumDrawCalls() { return m_NumDrawCalls; }; 
void SetNumDrawCalls(int value) { m_NumDrawCalls = value; }; 
void AddToNumDrawCalls(int value) { m_NumDrawCalls += value; };
private:
int m_ObjectID;
std::string m_Name;
int m_NumObjectMapHits = 0;
int m_NumDrawCalls = 0;
public:
static const int g_PropertyCount = 4;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_EngineStatsPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_NumObjectMapHits,
PIDX_NumDrawCalls,
};
bool Serialize(bool writing);

#pragma endregion  HEADER EngineStats

void ResetFrame();

};
