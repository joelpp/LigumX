#pragma once

#pragma region  FORWARD_DECLARATIONS Serializer2
#include "LXSystem.h"
class Serializer2;

class Serializer2;
class SerializerInputVariable;


#pragma endregion  FORWARD_DECLARATIONS Serializer2

#include "SerializerInputVariable.h"

#define FIND_VARIABLE() \
SerializerInputVariable serializerVariable; \
bool found = FindVariable(serializerVariable, varName); \
if (!found) \
{ \
	return; \
}

#define FIND_VARIABLE_SIZE(_size) \
FIND_VARIABLE(); \
lxAssert(serializerVariable.GetValues().size() == _size); \

#pragma region  HEADER Serializer2
class Serializer2 : public LXObject
{
public:
static const int ClassID = 2318800129;
static const LXType Type = LXType_Serializer2;
static constexpr const char* ClassName = "Serializer2";
typedef LXObject super;

bool GetWriting() { return m_Writing; }; 
void SetWriting(bool value) { m_Writing = value; }; 
bool GetIsValid() { return m_IsValid; }; 
void SetIsValid(bool value) { m_IsValid = value; }; 
const std::string& GetFilename() { return m_Filename; }; 
void SetFilename(std::string value) { m_Filename = value; }; 
std::vector<SerializerInputVariable>& GetSerializerInputVariables() { return m_SerializerInputVariables; }; 
void SetSerializerInputVariables(std::vector<SerializerInputVariable> value) { m_SerializerInputVariables = value; }; 
void AddTo_SerializerInputVariables(SerializerInputVariable value) { m_SerializerInputVariables.push_back(value); };
private:
bool m_Writing = false;
bool m_IsValid = false;
std::string m_Filename;
std::vector<SerializerInputVariable> m_SerializerInputVariables;
public:
static const int g_PropertyCount = 4;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_Serializer2PIDX
{
PIDX_Writing,
PIDX_IsValid,
PIDX_Filename,
PIDX_SerializerInputVariables,
};
void Serialize(Serializer2& serializer);
bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual const char* GetTypeName();

#pragma endregion  HEADER Serializer2

public:
	template<typename T>
	Serializer2(T* object, bool writing, const std::string& fileName)
		: m_Writing(writing)
		, m_Filename(fileName)
	{
		bool hardcodedId = g_ObjectManager->IsHardcodedID(object->GetObjectID());

		m_IsValid = !(m_Writing && hardcodedId);

		if (m_IsValid)
		{
			if (!m_Writing)
			{
				ProcessFile();
			}
		}
		else
		{
			lxAssert0();
		}
	}

	template<typename T>
	static Serializer2 Serializer2::CreateSerializer(T* object, bool writing)
	{	
		std::string basePath = g_PathObjects;
		std::string fileName = object->ClassName + std::string("_") + std::to_string(object->GetObjectID()) + ".LXobj";
		std::string fullPath = basePath + fileName;

		return Serializer2(object, writing, fullPath);

		/*
		if (writing)
		{
		return OutputSerializer(object, writing, fileName);
		}
		else
		{
		return InputSerializer(object, writing, fileName);
		}*/
	}

	void Close();

	void ProcessFile();

	bool FindVariable(SerializerInputVariable& var, const std::string& varName);

	void SerializeBool(const std::string& varName, bool& variable);
	void SerializeFloat(const std::string& varName, float& variable);
	void SerializeInt(const std::string& varName, int& variable);
	void SerializeInt64(const std::string& varName, lxInt64& variable);

	void SerializeString(const std::string& varName, std::string& variable);

	void SerializeIVec2(const std::string& varName, glm::ivec2& variable);
	void SerializeIVec3(const std::string& varName, glm::ivec3& variable);

	void SerializeVec2(const std::string& varName, glm::vec2& variable);
	void SerializeVec3(const std::string& varName, glm::vec3& variable);

	void SerializeIVec4(const std::string& varName, glm::ivec4& variable);
	void SerializeVec4(const std::string& varName, glm::vec4& variable);

	template <typename T>
	void SerializeVector(const std::string& varName, std::vector<T>& vec)
	{
		if (m_Writing)
		{

		}
		else
		{
			FIND_VARIABLE_SIZE(1);
		}
	}

	template <typename T>
	void SerializeVector(const std::string& varName, std::vector<T*>& vec)
	{
		if (m_Writing)
		{

		}
		else
		{
			FIND_VARIABLE();

			vec.clear();

			int vectorLength = StringUtils::ToInt(serializerVariable.GetValues()[0]);
			vec.resize(vectorLength);
			for (int i = 1; i < serializerVariable.GetValues().size(); ++i)
			{
				int objectID = StringUtils::ToInt(serializerVariable.GetValues()[i]);

				if (objectID == 0)
				{
					continue;
				}

				bool mustSerialize = false;
				T* loadedObject = g_ObjectManager->FindObjectByID<T>(objectID);

				//T* dptr = nullptr;
				if (loadedObject == nullptr)
				{
					// todo : prolly a huge leak here
					//if (ptr == 0)
					{
						loadedObject = new T();
						g_ObjectManager->AddObject(objectID, T::Type, (ObjectPtr)loadedObject);
					}
					//	mustSerialize = true;

					loadedObject->SetObjectID(objectID);
					loadedObject->Serialize(false);
				}
				else
				{

				}

				vec[i - 1] = loadedObject;

			}
		}
	}


	template <typename T>
	void Serializer2::SerializeObjectPtr(const std::string& varName, T*& ptr)
	{
		if (m_Writing)
		{

		}
		else
		{
			FIND_VARIABLE_SIZE(1);

			int objectID = StringUtils::ToInt(serializerVariable.GetValues()[0]);

			if (objectID == 0)
			{
				return;
			}

			bool mustSerialize = false;
			T* loadedObject = g_ObjectManager->FindObjectByID<T>(objectID);

			//T* dptr = nullptr;
			if (loadedObject == nullptr)
			{
				if (ptr == 0)
				{
					loadedObject = new T();
					g_ObjectManager->AddObject(objectID, T::Type, (ObjectPtr)loadedObject);
				}
			//	mustSerialize = true;
			}

			loadedObject->SetObjectID(objectID);
			loadedObject->Serialize(false);
			ptr = loadedObject;
			//else
			//{
			//	dptr = (T *)loadedObject;
			//}
			//char** aptr = (char**)ptr;
			//*aptr = (char*)dptr;
			//ptr = *(char**)ptr;
			//if (mustSerialize)
			//{
			//	T* val = (T *)ptr;
			//	val->SetObjectID(objectID);
			//	val->Serialize(false);
			//}
			//break;
		}
	}


private:


};
