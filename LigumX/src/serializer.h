#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "property.h"
#include "ObjectManager.h"
#include "Logging.h"
#include "Serializer2.h"


class Serializer
{
	ClassPropertyData idData = { "ObjectID", 0, 0, 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, 0, 0, 0, 0, };
	ClassPropertyData nameData = { "Name", 0, 0, 0, LXType_stdstring, sizeof(std::string), LXType_stdstring, false, LXType_None, 0, 0, 0, 0, };


public:
	Serializer::Serializer();

	template <typename T>
	void SerializeOnePropertyOut(T* object, const ClassPropertyData& propertyData, std::fstream& objectStream)
	{
		LXType lxType = propertyData.m_Type;

		if (propertyData.m_PropertyFlags & PropertyFlags_Transient)
		{
			return;
		}

		char* ptr = (char*)object + propertyData.m_Offset;;

		if (propertyData.IsAPointer)
		{
			ptr = *(char**)ptr;
		}

		if (lxType == LXType_stdvector)
		{
			std::vector<char*>* v = (std::vector<char*>*) ((char*)object + propertyData.m_Offset);

			objectStream << propertyData.m_Name << std::endl;
			objectStream << v->size() << std::endl;

			for (int i = 0; i < v->size(); ++i)
			{
				SerializePropertyOut((*v)[i], propertyData.m_Name, propertyData.m_AssociatedType, propertyData.m_SimpleType, objectStream);
			}
		}
		else
		{
			if (propertyData.m_PropertyFlags & PropertyFlags_Enum)
			{
				lxType = LXType_int;
			}

			objectStream << propertyData.m_Name << std::endl;
			SerializePropertyOut(ptr, propertyData.m_Name, lxType, propertyData.m_SimpleType, objectStream);
		}

	}

	template <typename T>
	void SerializeObjectOut(T* object, std::fstream& objectStream)
	{
		objectStream << "ObjectID" << std::endl;
		objectStream << object->GetObjectID() << std::endl;
		objectStream << "Name" << std::endl;
		objectStream << object->GetName() << std::endl;

		for (int i = 0; i < object->g_PropertyCount; ++i)
		{
			const ClassPropertyData& propertyData = object->g_Properties[i];

			SerializeOnePropertyOut(object, propertyData, objectStream);
		}
	}

	template <typename T>
	bool SerializeOnePropertyIn(T* object, std::string& line, const ClassPropertyData& propertyData, std::fstream& objectStream)
	{
		if (propertyData.m_PropertyFlags & PropertyFlags_Transient)
		{
			return true;
		}
		if (line != propertyData.m_Name)
		{
			return true;
		}

		if (propertyData.IsAPointer)
		{
			char* ptr = (char*)object + propertyData.m_Offset;

			SerializePropertyIn(ptr, propertyData.m_Type, propertyData.m_AssociatedType, propertyData.m_SimpleType, objectStream);
			return false;
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

					SerializePropertyIn(ptr, propertyData.m_AssociatedType, LXType_None, propertyData.m_SimpleType, objectStream);
				}

				return false;
			}
			else
			{
				char* ptr = (char*)object + propertyData.m_Offset;;

				LXType lxType = propertyData.m_Type;

				if (propertyData.m_PropertyFlags & PropertyFlags_Enum)
				{
					lxType = LXType_int;
				}

				SerializePropertyIn(ptr, lxType, propertyData.m_AssociatedType, propertyData.m_SimpleType, objectStream);
				return false;
			}
		}

	}


	template <typename T>
	void SerializeObjectIn(T* object, std::fstream& objectStream)
	{
		std::string line;

		std::getline(objectStream, line); // ObjectID
		std::getline(objectStream, line); // ObjectID value
		object->SetObjectID(std::atoi(line.c_str()));
		std::getline(objectStream, line); // name
		std::getline(objectStream, line); // name value
		object->SetName(line);

		while (std::getline(objectStream, line))
		{
			for (int i = 0; i < object->g_PropertyCount; ++i)
			{
				const ClassPropertyData& propertyData = object->g_Properties[i];
				
				bool shouldContinue = SerializeOnePropertyIn(object, line, propertyData, objectStream);

				if (shouldContinue)
				{
					continue;
				}
				else
				{
					break;
				}
			}
		}
	}


	template<typename T>
	bool SerializeObject(T* object, bool writing)
	{
		bool hardcodedId = g_ObjectManager->IsHardcodedID(object->GetObjectID());
		if (writing && hardcodedId)
		{
			return false;
		}

		std::string basePath = g_PathObjects;
		std::string fileName = object->ClassName + std::string("_") + std::to_string(object->GetObjectID()) + ".LXobj";
		std::string fullPath = basePath + fileName;
		int fileMask = writing ? std::ios::out : std::ios::in;
		std::fstream objectStream(fullPath, fileMask);

		if (objectStream.is_open())
		{
			SerializeObject(object, objectStream, writing);
		}
		else
		{
			OUTPUT_STRING("File " + fileName + " not found in objects bank.");

			return false;
		}

		return true;
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
	void SerializePropertyIn(char*& ptr, const LXType& type, const LXType& associatedType, const LXType& simpleType, std::fstream& objectStream);
	void SerializePropertyOut(const char* ptr, const char* name, const LXType& type, const LXType& simpleType, std::fstream& objectStream);
};

extern Serializer* g_Serializer;