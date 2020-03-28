#pragma once

#pragma region  FORWARD_DECLARATIONS EngineStats
#include "LXSystem.h"
class Serializer2;

class EngineStats;

extern EngineStats* g_EngineStats;

#pragma endregion  FORWARD_DECLARATIONS EngineStats

#pragma region  HEADER EngineStats
class EngineStats : public LXObject
{
public:
static const int ClassID = 204926310;
static const LXType Type = LXType_EngineStats;
static constexpr const char* ClassName = "EngineStats";
virtual const char* GetLXClassName() { return ClassName; }
typedef LXObject super;

int GetNumObjectMapHits() { return m_NumObjectMapHits; }; 
void SetNumObjectMapHits(int value) { m_NumObjectMapHits = value; }; 
void AddTo_NumObjectMapHits(int value) { m_NumObjectMapHits += value; };
int GetNumDrawCalls() { return m_NumDrawCalls; }; 
void SetNumDrawCalls(int value) { m_NumDrawCalls = value; }; 
void AddTo_NumDrawCalls(int value) { m_NumDrawCalls += value; };
private:
int m_NumObjectMapHits = 0;
int m_NumDrawCalls = 0;
public:
static const int g_PropertyCount = 2;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_EngineStatsPIDX
{
PIDX_NumObjectMapHits,
PIDX_NumDrawCalls,
};
virtual void Serialize(Serializer2& serializer);
bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual const char* GetTypeName();

#pragma endregion  HEADER EngineStats

void ResetFrame();

};
