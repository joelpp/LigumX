#pragma once

class Mesh;

class DefaultMeshes
{
public:
	DefaultMeshes();
	Mesh* DefaultQuadMesh;
	Mesh* DefaultSphereMesh;
	Mesh* DefaultCubeMesh;

	Mesh* GetMeshFromID(int id);

	void InitializeDefaultCube();
	void InitializeDefaultSphere();
	void InitializeDefaultQuad();
};

extern DefaultMeshes* g_DefaultMeshes;
