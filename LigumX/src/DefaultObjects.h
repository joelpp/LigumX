#pragma once

class Mesh;
class Model;
class Entity;
class Material;

class DefaultObjects
{
public:
	DefaultObjects();
	Mesh* DefaultQuadMesh;
	Mesh* DefaultSphereMesh;
	Mesh* DefaultCubeMesh;

	Mesh* DefaultTerrainMesh;

	Mesh* DefaultLineMesh;

	Mesh* GetMeshFromID(int id);

	Material* DefaultRedMaterial;
	Model* DefaultCubeModel;
	Entity* DefaultManipulatorEntity;

	void InitializeManipulator();


	void InitializeDefaultCube();
	void InitializeDefaultSphere();
	void InitializeDefaultQuad();
	void InitializeDefaultLine();

	void InitializeDefaultTerrainMesh();
};

extern DefaultObjects* g_DefaultObjects;
