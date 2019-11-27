#pragma once
#include "..\..\nclgl\SceneNode.h" 
#include "..\..\nclgl\OBJMesh.h" 

class SmallIsland : public SceneNode {
public:
	SmallIsland(void);
	~SmallIsland(void) {};

	virtual void Update(float msec);

	static void CreateIsland() {
		OBJMesh* m = new OBJMesh();
		m->LoadOBJMesh(MESHDIR"Small Tropical Island.obj");
		Island = m;
	}

	static void DeleteIsland() { delete Island; }
protected:
	static Mesh* Island; //

};