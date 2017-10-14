#include "property.h"
#include <fstream>
#include <string>

class Serializer
{

public:
	template<typename T>
	static void SerializeObject(T* object, std::fstream& objectStream, bool writing)
	{
		if (writing)
		{
			for (int i = 0; i < object->g_PropertyCount; ++i)
			{
				const ClassPropertyData& data = object->g_Properties[i];
				SerializePropertyOut((char*)object + data.m_Offset, data.m_Name, data.m_Type, objectStream);
			}
		}
		else
		{
			std::string line;
			while (std::getline(objectStream, line))
			{
				if (line == "")
				{
					continue;
				}

				for (int i = 0; i < object->g_PropertyCount; ++i)
				{
					const ClassPropertyData& data = object->g_Properties[i];
					if (line == data.m_Name)
					{
						SerializePropertyIn((char*)object + data.m_Offset, data.m_Type, objectStream);
						break;
					}
				}
			}
		}

	}

private:

	static void SerializePropertyIn(const char* ptr, const LXType& type, std::fstream& objectStream);
	static void SerializePropertyOut(const char* ptr, const char* name, const LXType& type, std::fstream& objectStream);
};
