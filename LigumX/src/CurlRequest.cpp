#include "CurlRequest.h"

#pragma warning( disable : 4005 ) // dunno how to silence this one dammit. warning C4005: 'APIENTRY': macro redefinition
#include "curl/curl.h"
#include "Sector.h"
#include "tinyxml2\tinyxml2.h"

#pragma region  CLASS_SOURCE CurlRequest

#include "CurlRequest.h"
#include "serializer.h"
#include "Sector.h"
const ClassPropertyData CurlRequest::g_Properties[] = 
{
{ "State", PIDX_State, offsetof(CurlRequest, m_State), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, 0, (float)LX_LIMITS_INT_MIN, (float)LX_LIMITS_INT_MAX, 0,}, 
{ "Coords", PIDX_Coords, offsetof(CurlRequest, m_Coords), 0, LXType_glmvec2, sizeof(glm::vec2), LXType_glmvec2, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "Extent", PIDX_Extent, offsetof(CurlRequest, m_Extent), 0, LXType_glmvec2, sizeof(glm::vec2), LXType_glmvec2, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "Result", PIDX_Result, offsetof(CurlRequest, m_Result), 0, LXType_stdstring, sizeof(std::string), LXType_stdstring, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "Filename", PIDX_Filename, offsetof(CurlRequest, m_Filename), 0, LXType_stdstring, sizeof(std::string), LXType_stdstring, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "Sector", PIDX_Sector, offsetof(CurlRequest, m_Sector), 0, LXType_ObjectPtr, sizeof(Sector*), LXType_Sector, true, LXType_None, false, 0, 0, 0, 0,}, 
{ "SectorIndex", PIDX_SectorIndex, offsetof(CurlRequest, m_SectorIndex), 0, LXType_glmivec2, sizeof(glm::ivec2), LXType_glmivec2, false, LXType_None, false, 0, (float)LX_LIMITS_INT_MIN, (float)LX_LIMITS_INT_MAX, 0,}, 
{ "Async", PIDX_Async, offsetof(CurlRequest, m_Async), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
};
void CurlRequest::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeInt(g_Properties[PIDX_State], m_State);
	serializer.SerializeVec2(g_Properties[PIDX_Coords], m_Coords);
	serializer.SerializeVec2(g_Properties[PIDX_Extent], m_Extent);
	serializer.SerializeString(g_Properties[PIDX_Result], m_Result);
	serializer.SerializeString(g_Properties[PIDX_Filename], m_Filename);
	serializer.SerializeObjectPtr(g_Properties[PIDX_Sector], m_Sector);
	serializer.SerializeIVec2(g_Properties[PIDX_SectorIndex], m_SectorIndex);
	serializer.SerializeBool(g_Properties[PIDX_Async], m_Async);
}
bool CurlRequest::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool CurlRequest::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_State], &m_State , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_Coords], &m_Coords , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_Extent], &m_Extent , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_Result], &m_Result  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_Filename], &m_Filename  );
	ImguiHelpers::ShowObject2(this, g_Properties[PIDX_Sector], &m_Sector  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_SectorIndex], &m_SectorIndex , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_Async], &m_Async  );
	return true;
}
const char* CurlRequest::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE CurlRequest


size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

std::string curl_request(char* path)
{
#ifndef NOCURL
	CURL *curl_handle;
	CURLcode res;

	std::string result;

	curl_handle = curl_easy_init();
	if (curl_handle)
	{
		curl_easy_setopt(curl_handle, CURLOPT_URL, path);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &result);

		res = curl_easy_perform(curl_handle);

		/* always cleanup */
		curl_easy_cleanup(curl_handle);
	}
	return result;
#else
	return "";
#endif

}

const char* g_OSM_API_Path = "http://www.overpass-api.de/api/xapi_meta?*[bbox=%.7f,%.7f,%.7f,%.7f]";

std::string queryBoundingBox(float left, float bottom, float right, float top)
{
	char path[256];
	sprintf(path, g_OSM_API_Path, left, bottom, right, top);
	return curl_request(path);
}

std::string queryContourData(float left, float bottom, float right, float top, std::string savePath)
{
	std::stringstream request;
	request << "cd /Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/tools/Srtm2Osm && \
                          mono Srtm2Osm.exe -bounds1 ";
	request << bottom << " ";
	request << left << " ";
	request << top << " ";
	request << right << " ";

	request << "-o ";
	request << savePath;

	system(request.str().c_str());

	std::ifstream t(savePath.c_str());
	std::stringstream buffer;
	buffer << t.rdbuf();

	return buffer.str();
}

std::string downloadSectorData(glm::vec2 coords)
{
	float sectorSize = Settings::GetInstance().f("sectorSize");
	glm::vec2 shift = Settings::GetInstance().f2("coordinateShifting");
	return queryBoundingBox(coords.x - shift.x,
		coords.y - shift.y,
		coords.x + sectorSize - shift.x,
		coords.y + sectorSize - shift.y);
}


std::string downloadContourData(glm::vec2 coords, std::string savePath)
{
	float sectorSize = Settings::GetInstance().f("sectorSize");
	glm::vec2 shift = Settings::GetInstance().f2("coordinateShifting");
	return queryContourData(coords.x - shift.x,
		coords.y - shift.y,
		coords.x + sectorSize - shift.x,
		coords.y + sectorSize - shift.y,
		savePath);
}

inline bool file_exists(const std::string& name)
{
	if (FILE *file = fopen(name.c_str(), "r"))
	{
		fclose(file);
		return true;
	}
	else
	{
		return false;
	}
}


std::string BuildXMLPath(int dataType, const std::string& fileName)
{
	std::stringstream savePath;
	savePath << "C:/Users/Joel/Documents/LigumX/LigumX/data/";
	switch (dataType)
	{
	case 0:   savePath << "SRTMData/";
		break;
	case 1:       savePath << "OSMData/";
	}

	int index = 1;

	savePath << fileName;
	savePath << ".xml";

	return savePath.str();
}

CurlRequest::CurlRequest()
	:	m_State(ThreadState_Ready),
		m_Sector(nullptr),
		m_SectorIndex(glm::ivec2(0, 0))
{
}


CurlRequest::CurlRequest(glm::vec2 coords, glm::vec2 extent, bool async, const std::string& fileName)
	:	m_Coords(coords),
		m_Extent(extent),
		m_Sector(nullptr),
		m_State(ThreadState_Ready),
		m_SectorIndex(glm::ivec2(0, 0)),
		m_Async(async)

{
	m_Filename = BuildXMLPath(0, fileName);

}


void CurlRequest::Execute()
{
	PRINT("Beginning cURL request.");
	std::cout << "Searching for " << m_Filename << "..." << std::endl;

	bool fileAlreadyPresent = file_exists(m_Filename);

	if (!fileAlreadyPresent)
	{
		std::cout << "File not found on disk. Downloading from OpenStreeMaps..." << std::endl;
		m_Result = queryBoundingBox(m_Coords.x, m_Coords.y, m_Coords.x + m_Extent.x, m_Coords.y + m_Extent.y);
		StringUtils::DumpToFile(m_Filename, m_Result);
	}
	else
	{
		std::cout << "File found on disk!" << std::endl;
	}

	PRINT("Request complete")
	m_State = ThreadState_Finished;
}
	 


void CurlRequest::Start()
{
	if (m_Async)
	{
		m_JobThread = std::thread(&CurlRequest::Execute, this);
		m_State = ThreadState_Ready;
	}
	else
	{
		Execute();
	}

}

void CurlRequest::End()
{
	if (m_Async)
	{
		m_JobThread.join();
	}
}
