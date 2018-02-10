#pragma once

#include <unordered_map>
#include <sstream>
#include <vector>
#include <string>

#include "glm/glm.hpp"

#include "Logging.h"

class Settings
{
public:

	std::string get(std::string key)
	{
		return m_settings[key];
	}

	void add(std::string key, std::string value)
	{
		m_settings.emplace(key, value);
	}

	void add(std::string key, glm::vec2 vec);

	std::string getFormatted(std::string key);

	void printAll()
	{
		for (auto it = m_settings.begin(); it != m_settings.end(); ++it)
		{
			PRINTSTRING(getFormatted(it->first));
		}
	}

	bool load();

	glm::vec3 f3(std::string key);
	glm::vec2 f2(std::string key);
	float f(std::string key);
	int i(std::string key);
	glm::vec2 i2(std::string key);
	std::string s(std::string key);
	bool b(std::string key);

   static Settings& GetInstance()
    {
        static Settings instance; // Guaranteed to be destroyed.
                              	  // Instantiated on first use.
        return instance;
    }
private:
	REGISTERCLASS(Settings);
	std::unordered_map<std::string, std::string> m_settings;

    Settings() {};                   // Constructor? (the {} brackets) are needed here.

    // C++ 11
    // =======
    // We can use the better technique of deleting the methods
    // we don't want.
    Settings(Settings const&)               = delete;
    void operator=(Settings const&)  = delete;
};