#pragma once

#include <sstream>
#include <iostream>
#include <fstream>
#include "glm/glm.hpp"
#include "curl/curl.h"
#include "Settings.h"
#include "Logging.h"

#pragma region  FORWARD_DECLARATIONS CurlRequest
#include "property.h"

class CurlRequest;


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
const bool& GetFinished() { return m_Finished; }; 
void SetFinished(bool value) { m_Finished = value; }; 
const glm::vec2& GetCoords() { return m_Coords; }; 
void SetCoords(glm::vec2 value) { m_Coords = value; }; 
const glm::vec2& GetExtent() { return m_Extent; }; 
void SetExtent(glm::vec2 value) { m_Extent = value; }; 
const std::string& GetResult() { return m_Result; }; 
void SetResult(std::string value) { m_Result = value; }; 
private:
int m_ObjectID;
std::string m_Name;
bool m_Finished;
glm::vec2 m_Coords;
glm::vec2 m_Extent;
std::string m_Result;
public:
static const int g_PropertyCount = 6;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_CurlRequestPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_Finished,
PIDX_Coords,
PIDX_Extent,
PIDX_Result,
};
bool Serialize(bool writing);

#pragma endregion  HEADER CurlRequest

	CurlRequest(glm::vec2 coords, glm::vec2 extent);

	void Execute();

};
