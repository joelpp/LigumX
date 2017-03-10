#include <sstream>
#include <iostream>
#include <fstream>

#include "glm/glm.hpp"

#define NOCURL

#ifndef NOCURL
#include "curl/curl.h"
#endif

#include "Settings.h"
#include "Logging.h"

REGISTERCLASS("DataRequest");
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
  	if(curl_handle)
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
	char path[100];
	sprintf(path, "http://api.openstreetmap.fr/xapi?*[bbox=%f.2,%f.2,%f.2,%f.2]", left, bottom, right, top);

  std::cout << path;
	return curl_request(path);
}

std::string queryContourData(float left, float bottom, float right, float top, std::string savePath)
{
  std::stringstream request;
  request << "cd /Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/tools/Srtm2Osm && \
                          mono Srtm2Osm.exe -bounds1 ";
  request << bottom << " ";
  request << left   << " ";
  request << top    << " ";
  request << right  << " ";

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
  std::cout << "DOWNLOADING DATA......." << coords.x << coords.x + sectorSize << "\n";
  return queryBoundingBox(coords.x - shift.x, 
                          coords.y - shift.y, 
                          coords.x + sectorSize - shift.x, 
                          coords.y + sectorSize - shift.y);
}


std::string downloadContourData(glm::vec2 coords, std::string savePath)
{
  float sectorSize = Settings::GetInstance().f("sectorSize");
  glm::vec2 shift = Settings::GetInstance().f2("coordinateShifting");
  std::cout << "DOWNLOADING DATA......." << coords.x << coords.x + sectorSize << "\n";
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
