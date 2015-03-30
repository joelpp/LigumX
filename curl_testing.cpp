// my first program in C++
#include <iostream>
#include <curl/curl.h>
#include <string>
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
	sprintf(path, "http://open.mapquestapi.com/xapi/api/0.6/map?bbox=%f,%f,%f,%f", left, bottom, right, top);
	cout << path;
	return curl_request(path);
}

int main()
{
	// string s = queryBoundingBox(-73.1,45,-73.0,45.1);

	tinyxml2::XMLDocument doc;

	doc.LoadFile("sampleQuery.xml");
	XMLElement* docRoot = doc.RootElement();
	// doc.Parse(s.c_str());
	
	// cout << s;
	cout << "Hello World!\n";
}
