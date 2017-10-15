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
				const ClassPropertyData& propertyData = object->g_Properties[i];

				if (propertyData.m_PropertyFlags & PropertyFlags_Transient)
				{
					continue;
				}

				char* ptr = (char*)object + propertyData.m_Offset;;

				if (propertyData.IsAPointer)
				{
					ptr = *(char**)ptr;
				}

				SerializePropertyOut(ptr, propertyData.m_Name, propertyData.m_Type, objectStream);
			}
		}
		else
		{
			// first serialize pointers
			for (int i = 0; i < object->g_PropertyCount; ++i)
			{
				const ClassPropertyData& propertyData = object->g_Properties[i];
				if ( !(propertyData.IsAPointer) || (propertyData.m_PropertyFlags & PropertyFlags_Transient) )
				{
					continue;
				}

				char* ptr = (char*)object + propertyData.m_Offset;
				ptr = *(char**)ptr;

				SerializePropertyIn(ptr, propertyData.m_Type, objectStream);
			}

			// then serialize class variables
			std::string line;
			while (std::getline(objectStream, line))
			{
				if (line == "")
				{
					continue;
				}

				for (int i = 0; i < object->g_PropertyCount; ++i)
				{
					const ClassPropertyData& propertyData = object->g_Properties[i];
					if (propertyData.IsAPointer || line != propertyData.m_Name || (propertyData.m_PropertyFlags & PropertyFlags_Transient))
					{
						continue;
					}

					char* ptr = (char*)object + propertyData.m_Offset;;
					SerializePropertyIn(ptr, propertyData.m_Type, objectStream);
					break;
				}
			}
		}

	}

private:

	static void SerializePropertyIn(const char* ptr, const LXType& type, std::fstream& objectStream);
	static void SerializePropertyOut(const char* ptr, const char* name, const LXType& type, std::fstream& objectStream);
};
