#pragma once

#include"../../nclgl/OGLRenderer.h"
#include"../../nclgl/Camera.h"
#include"../../nclgl/HeightMap.h"
#include"../../nclgl/MD5Mesh.h"
#include"../../nclgl/MD5Node.h"

#define SHADOWSIZE 2048

class Renderer : public OGLRenderer {
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);

protected:
	void DrawHeightmap();
	void DrawWater();
	void DrawSkybox();
	void DrawMesh(); // New !
	void DrawFloor(); // New !
	void DrawShadowScene(); // New !
	void DrawCombinedScene(); // New !

	Shader* sceneShader;
	Shader* shadowShader;

	GLuint shadowTex;
	GLuint shadowFBO;

	MD5FileData* hellData;
	MD5Node* hellNode;
	Mesh* floor;

	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;

	HeightMap* heightMap;
	Mesh* quad;

	Light* light;
	Camera* camera;

	GLuint cubeMap;

	float waterRotate;
};