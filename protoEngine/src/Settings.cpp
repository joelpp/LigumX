#include "Settings.h"

typedef std::vector<std::string> StringVector;

const std::vector<std::string> explode(const std::string& s, const char& c) //thanks to cplusplus.com
{
	std::string buff{""};
	std::vector<std::string> v;
	
	for(auto n:s)
	{
		if(n != c) buff+=n; else
		if(n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if(buff != "") v.push_back(buff);
	
	return v;
}

bool Settings::load()
{
	std::string key, value, line;
	std::ifstream file;
	file.open("/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/data/settings.dat");

	while (std::getline(file, line))
	{
		PRINT(line);

		if ( (line.size() == 0) || (line.at(0) == ';') )
		{
			continue;
		}
		PRINTSTRING("passed!");
		
		StringVector v{explode(line, ' ')};

		key = v[0];

		std::stringstream ss;
		for(int i = 1; i < v.size(); ++i)
		{
			ss << v[i] << " ";
		}
		value = ss.str();
		std::cout << key;
		std::cout << value;
		m_settings.emplace(key, value);
	}

}

std::string Settings::getFormatted(std::string key)
{
		std::stringstream ss;
		ss << key;
		ss << " : ";
		ss << m_settings[key];
		return ss.str();
}

glm::vec3 Settings::f3(std::string key)
{
	StringVector v{ explode(m_settings[key], ' ') };

	glm::vec3 vValue;
	vValue.x = std::atof(v[0].c_str());
	vValue.y = std::atof(v[1].c_str());
	vValue.z = std::atof(v[2].c_str());

	return vValue;
}

glm::vec2 Settings::f2(std::string key)
{
	StringVector v{ explode(m_settings[key], ' ') };

	glm::vec2 vValue;
	vValue.x = std::atof(v[0].c_str());
	vValue.y = std::atof(v[1].c_str());

	return vValue;
}
float Settings::f(std::string key){
	return std::atof(m_settings[key].c_str());
}

int Settings::i(std::string key){
	return std::atoi(m_settings[key].c_str());
}