#pragma once


enum ThreadState
{
	ThreadState_Ready = 0,
	ThreadState_Initialized,
	ThreadState_Finished
};

#pragma region  FORWARD_DECLARATIONS CurlRequest
#include "LXSystem.h"
class Serializer2;

class CurlRequest;
class Sector;


#pragma endregion  FORWARD_DECLARATIONS CurlRequest

#include "Settings.h"



#pragma region  HEADER CurlRequest
class CurlRequest : public LXObject
{
public:
static const int ClassID = 2618865284;
static const LXType Type = LXType_CurlRequest;
static constexpr const char* ClassName = "CurlRequest";
typedef LXObject super;

int GetState() { return m_State; }; 
void SetState(int value) { m_State = value; }; 
glm::vec2& GetCoords() { return m_Coords; }; 
void SetCoords(const glm::vec2& value) { m_Coords = value; }; 
glm::vec2& GetExtent() { return m_Extent; }; 
void SetExtent(const glm::vec2& value) { m_Extent = value; }; 
const std::string& GetResult() { return m_Result; }; 
void SetResult(std::string value) { m_Result = value; }; 
const std::string& GetFilename() { return m_Filename; }; 
void SetFilename(std::string value) { m_Filename = value; }; 
Sector*& GetSector() { return m_Sector; }; 
void SetSector(Sector* value) { m_Sector = value; }; 
glm::ivec2& GetSectorIndex() { return m_SectorIndex; }; 
void SetSectorIndex(const glm::ivec2& value) { m_SectorIndex = value; }; 
bool GetAsync() { return m_Async; }; 
void SetAsync(bool value) { m_Async = value; }; 
private:
int m_State = 0;
glm::vec2 m_Coords = glm::vec2(0, 0);
glm::vec2 m_Extent = glm::vec2(0, 0);
std::string m_Result;
std::string m_Filename;
Sector* m_Sector = nullptr;
glm::ivec2 m_SectorIndex = glm::ivec2(0, 0);
bool m_Async = false;
public:
static const int g_PropertyCount = 8;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_CurlRequestPIDX
{
PIDX_State,
PIDX_Coords,
PIDX_Extent,
PIDX_Result,
PIDX_Filename,
PIDX_Sector,
PIDX_SectorIndex,
PIDX_Async,
};
bool Serialize(Serializer2& serializer);
bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual const char* GetTypeName();

#pragma endregion  HEADER CurlRequest

CurlRequest();
CurlRequest(glm::vec2 coords, glm::vec2 extent, bool async);

void Execute();

bool Ready() { return m_State == ThreadState_Ready; }
bool Finished() { return m_State == ThreadState_Finished; }
void Reset() { m_State = ThreadState_Ready; };
void Initialize() { m_State = ThreadState_Initialized; };

void Start();
void End();
private:

	std::thread m_JobThread;



};
