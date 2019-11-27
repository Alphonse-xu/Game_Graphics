#pragma once

#include"../../nclgl/OGLRenderer.h"
#include"../../nclgl/Camera.h"
#include"../../nclgl/HeightMap.h"
#include"../../nclgl/MD5Mesh.h"
#include"../../nclgl/MD5Node.h"
#include"../../nclgl/SceneNode.h"
#include "../../nclgl/ParticleEmitter.h"
#include"Island.h"

#define SHADOWSIZE 2048
#define POST_PASSES 10

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
	void DrawMesh(); 
	void DrawFloor(); 
	void DrawShadowScene(); 
	void DrawParticle(); 
	void DrawCombinedScene(); 
	void DrawNode(SceneNode* n);
	int ScreenLock();
	void SetShaderParticleSize(float f);	

	ParticleEmitter* emitter;	//A single particle emitter
	SceneNode* root;

	Shader* sceneShader;
	Shader* shadowShader;
	Shader* particleShader;


	GLuint shadowTex;
	GLuint shadowFBO;

	MD5FileData* hellData;
	MD5Node* hellNode;
	Mesh* floor;

	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;
	Shader* IslandShader;

	HeightMap* heightMap;
	Mesh* quad;

	Light* light;
	Camera* camera;

	GLuint cubeMap;

	float waterRotate;
	int waterheight;

	int mflag=0;
	float Rmsec=1.0f;

	//void PresentScene();
	//void DrawPostProcess();
	//void DrawScene();
	//GLuint bufferFBO;
	//GLuint processFBO; //后期模糊帧缓冲
	//GLuint bufferColorTex[2];
	//GLuint bufferDepthTex;

	//Shader* processShader; //
};