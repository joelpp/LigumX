#pragma once

#include <sstream>
#include <thread>
#include <iostream>
#include <fstream>
#include "glm/glm.hpp"
#include "Settings.h"
#include "Logging.h"

enum ThreadState
{
	ThreadState_Ready = 0,
	ThreadState_Initialized,
	ThreadState_Finished
};

#pragma region  FORWARD_DECLARATIONS CurlRequest
#include "property.h"

class CurlRequest;
class Sector;


#pragma endregion  FORWARD_DECLARATIONS CurlRequest


class CurlRequest
{
#pragma region  HEADER CurlRequest
public:
static const int ClassID = 2618865284;
static const LXType Type = LXType_CurlRequest;
static constexpr const char* ClassName = "CurlRequest";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
const int& GetState() { return m_State; }; 
void SetState(int value) { m_State = value; }; 
const glm::vec2& GetCoords() { return m_Coords; }; 
void SetCoords(glm::vec2 value) { m_Coords = value; }; 
const glm::vec2& GetExtent() { return m_Extent; }; 
void SetExtent(glm::vec2 value) { m_Extent = value; }; 
const std::string& GetResult() { return m_Result; }; 
void SetResult(std::string value) { m_Result = value; }; 
const std::string& GetFilename() { return m_Filename; }; 
void SetFilename(std::string value) { m_Filename = value; }; 
Sector*& GetSector() { return m_Sector; }; 
void SetSector(Sector* value) { m_Sector = value; }; 
const glm::ivec2& GetSectorIndex() { return m_SectorIndex; }; 
void SetSectorIndex(glm::ivec2 value) { m_SectorIndex = value; }; 
private:
int m_ObjectID;
std::string m_Name;
int m_State = 0;
glm::vec2 m_Coords = glm::vec2(0, 0);
glm::vec2 m_Extent = glm::vec2(0, 0);
std::string m_Result;
std::string m_Filename;
Sector* m_Sector;
glm::ivec2 m_SectorIndex;
public:
static const int g_PropertyCount = 9;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_CurlRequestPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_State,
PIDX_Coords,
PIDX_Extent,
PIDX_Result,
PIDX_Filename,
PIDX_Sector,
PIDX_SectorIndex,
};
bool Serialize(bool writing);

#pragma endregion  HEADER CurlRequest

CurlRequest();
CurlRequest(glm::vec2 coords, glm::vec2 extent);

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
