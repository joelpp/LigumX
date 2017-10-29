#pragma once

#include "property.h"
#include "ObjectManager.h"
#include "DisplayOptions.h"
#include <fstream>
#include <string>
#include <vector>

class Serializer
{

public:
	Serializer::Serializer();

	template <typename T>
	void SerializeObjectOut(T* object, std::fstream& objectStream)
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

			if (propertyData.m_Type == LXType_stdvector)
			{
				std::vector<char*>* v = (std::vector<char*>*) ((char*)object + propertyData.m_Offset);

				objectStream << propertyData.m_Name << std::endl;
				objectStream << v->size() << std::endl;

				for (int i = 0; i < v->size(); ++i)
				{
					SerializePropertyOut((*v)[i], propertyData.m_Name, propertyData.m_AssociatedType, objectStream);
				}
			}
			else
			{
				objectStream << propertyData.m_Name << std::endl;
				SerializePropertyOut(ptr, propertyData.m_Name, propertyData.m_Type, objectStream);
			}

		}
	}

	template <typename T>
	void SerializeObjectIn(T* object, std::fstream& objectStream)
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
				const ClassPropertyData& propertyData = object->g_Properties[i];
				if (propertyData.m_PropertyFlags & PropertyFlags_Transient)
				{
					continue;
				}
				if (line != propertyData.m_Name)
				{
					continue;
				}

				if (propertyData.IsAPointer)
				{
					char* ptr = (char*)object + propertyData.m_Offset;

					SerializePropertyIn(ptr, propertyData.m_Type, propertyData.m_AssociatedType, objectStream);
					break;
				}
				else
				{
					if (propertyData.m_Type == LXType_stdvector)
					{
						std::getline(objectStream, line);
						int numItems = std::atoi(line.c_str());

						std::vector<char*>* v = (std::vector<char*>*) ((char*)object + propertyData.m_Offset);
						v->resize(numItems);
						for (int i = 0; i < numItems; ++i)
						{
							char* ptr = (char*)(&((*v)[i]));

							SerializePropertyIn(ptr, propertyData.m_AssociatedType, LXType_None, objectStream);
						}

						break;
					}
					else
					{
						char* ptr = (char*)object + propertyData.m_Offset;;
						SerializePropertyIn(ptr, propertyData.m_Type, propertyData.m_AssociatedType, objectStream);
						break;
					}
				}
				

			}
		}
	}


	template<typename T>
	void SerializeObject(T* object, bool writing)
	{
		std::string basePath = g_PathObjects;
		std::string fileName = object->ClassName + std::string("_") + std::to_string(object->GetObjectID()) + ".LXobj";

		int fileMask = writing ? std::ios::out : std::ios::in;
		std::fstream objectStream(basePath + fileName, fileMask);

		if (objectStream.is_open())
		{
			SerializeObject(object, objectStream, writing);
		}
	}

	template<typename T>
	void SerializeObject(T* object, std::fstream& objectStream, bool writing)
	{
		if (writing)
		{
			SerializeObjectOut(object, objectStream);
		}
		else
		{
			SerializeObjectIn(object, objectStream);
		}
	}

	void BackupData();
private:
	void SerializePropertyIn(char*& ptr, const LXType& type, const LXType& associatedType, std::fstream& objectStream);
	void SerializePropertyOut(const char* ptr, const char* name, const LXType& type, std::fstream& objectStream);
};

extern Serializer* g_Serializer;