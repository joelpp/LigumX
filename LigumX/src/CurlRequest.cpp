#include "CurlRequest.h"
#include "tinyxml2\tinyxml2.h"
#include "curl/curl.h"
#include "Logging.h"
#include "StringUtils.h"
#pragma region  CLASS_SOURCE CurlRequest

#include "CurlRequest.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData CurlRequest::g_Properties[] = 
{
{ "ObjectID", PIDX_ObjectID, offsetof(CurlRequest, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "Name", PIDX_Name, offsetof(CurlRequest, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
{ "State", PIDX_State, offsetof(CurlRequest, m_State), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "Coords", PIDX_Coords, offsetof(CurlRequest, m_Coords), 0, LXType_glmvec2, false, LXType_None, 0, 0, 0, }, 
{ "Extent", PIDX_Extent, offsetof(CurlRequest, m_Extent), 0, LXType_glmvec2, false, LXType_None, 0, 0, 0, }, 
{ "Result", PIDX_Result, offsetof(CurlRequest, m_Result), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
{ "Filename", PIDX_Filename, offsetof(CurlRequest, m_Filename), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
};
bool CurlRequest::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
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

std::string queryBoundingBox(float left, float bottom, float right, float top)
{
	char path[256];
	sprintf(path, "http://overpass.osm.rambler.ru/cgi/xapi_meta?*[bbox=%f.2,%f.2,%f.2,%f.2]", left, bottom, right, top);
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


CurlRequest::CurlRequest()
	: m_State(0)
{
}


CurlRequest::CurlRequest(glm::vec2 coords, glm::vec2 extent)
	:	m_Coords(coords),
		m_Extent(extent),
		m_State(0)
{
}

std::string BuildXMLPath(int dataType, glm::vec2 pos) {
	std::stringstream savePath;
	//savePath << "/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/data/";
	savePath << "C:/Users/Joel/Documents/LigumX/LigumX/data/";
	switch (dataType)
	{
	case 0:   savePath << "SRTMData/";
		break;
	case 1:       savePath << "OSMData/";
	}

	int index = 1;

	savePath << (int) (pos.x * 1000) << "x" << (int)(pos.y * 1000);
	savePath << ".xml";

	return savePath.str();
}

void CurlRequest::Execute()
{
	PRINTSTRING("Beginning cURL request.");

	m_Filename = BuildXMLPath(0, m_Coords);

	bool fileAlreadyPresent = file_exists(m_Filename);

	if (!fileAlreadyPresent)
	{
		m_Result = queryBoundingBox(m_Coords.x, m_Coords.y, m_Coords.x + m_Extent.x, m_Coords.y + m_Extent.y);
		StringUtils::DumpToFile(m_Filename, m_Result);
	}

	PRINTSTRING("Request complete")
	m_State = 2;
}
	 

