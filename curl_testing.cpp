// my first program in C++
#include <iostream>
#include <curl/curl.h>
#include <string>
#include <sstream>
#include <fstream>
#include "lib/tinyxml2.h"

using namespace std;

// Provided by http://stackoverflow.com/questions/9786150/save-curl-content-result-into-a-string-in-c
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

string curl_request(char* path){
	CURL *curl_handle;
  	CURLcode res;

	string result;

  	curl_handle = curl_easy_init();
  	if(curl_handle) {
  	  	curl_easy_setopt(curl_handle, CURLOPT_URL, path);
  	  	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteCallback);
  	  	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &result);

  	  	res = curl_easy_perform(curl_handle);

	    /* always cleanup */
		curl_easy_cleanup(curl_handle);
    }
    return result;
}

string queryBoundingBox(float left, float bottom, float right, float top){
	char path[100];
	// sprintf(path, "http://api.openstreetmap.fr/xapi?*[bbox=%f,%f,%f,%f]", left, bottom, right, top);
	sprintf(path, "http://api.openstreetmap.fr/xapi?*[bbox=%f,%f,%f,%f]", left, bottom, right, top);
	
	cout << path;
	return curl_request(path);
}

int main()
{
	float step = 0.01;
	float minLon = -73.65;
	float maxLon = -73.64;
	float minLat = 45.5;
	float maxLat = 45.51; 

	int index = 0;
	for (int i = 0; i < 3; i++){
		minLat = 45.5;
		maxLat = 45.51; 

		for (int j = 0; j < 3; j++){
			string s = queryBoundingBox(minLon, minLat, maxLon, maxLat);
			std::stringstream ss;
			ss << "./protoEngine/data/Data_";
			ss << minLon * 1000 << "x" << minLat * 1000;
			ss << ".xml";
			std::ofstream out(ss.str());
		    out << s;
		    out.close();
		    minLat += step;
			maxLat += step;
			std::cout << minLon << " " << maxLon << " " << minLat << " " << maxLat << "\n";
			index++;
			break;
		}
		break;
		minLon = maxLon;
		maxLon += step;
	}

	// tinyxml2::XMLDocument doc;

	// doc.LoadFile("sampleQuery.xml");
	// XMLElement* docRoot = doc.RootElement();
	// doc.Parse(s.c_str());
	
	// cout << s;
	cout << "Hello World!\n";
}

