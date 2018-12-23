#pragma once

#include "LXSystem.h"

class LXObject
{
public:
	int GetObjectID() { return m_ObjectID; };
	void SetObjectID(int objectID) { m_ObjectID = objectID; };
	const LXString& GetName() { return m_Name; }
	void SetName(const LXString& name) { m_Name = name; }
private:
	int m_ObjectID;
	LXString m_Name;
};