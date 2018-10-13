#pragma once

#include <string>

class LXObject
{
public:
	int GetObjectID() { return m_ObjectID; };
	void SetObjectID(int objectID) { m_ObjectID = objectID; };
	const std::string& GetName() { return m_Name; }
	void SetName(const std::string& name) { m_Name = name; }
private:
	int m_ObjectID;
	std::string m_Name;
};