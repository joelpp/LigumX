#pragma once

class ObjectIDManager
{
public:
	ObjectIDManager();

	int GetObjectID();
	int GetTransientID();

	bool IsValidID(int id) { return id == InvalidID; }
	bool IsHardcodedID(int id) { return id >= StartHardcodedIDs && id < StartTransientIDs;  }



	const int InvalidID = 0;
	const int StartHardcodedIDs = 1;
	const int StartTransientIDs = 2048;
	const int StartObjectIDs = 8192;

	int DefaultSphereMeshID;
	int DefaultQuadMeshID;
	int DefaultCubeMeshID;

private:
	int m_NextTransientID = StartTransientIDs;
	int m_NextHardcodedID;
};

extern ObjectIDManager* g_ObjectIDManager;
