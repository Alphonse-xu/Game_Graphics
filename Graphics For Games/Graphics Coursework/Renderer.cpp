#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	camera = new Camera();
	light = new Light(Vector3(-450.0f, 200.0f, 280.0f),
		Vector4(1, 1, 1, 1), 5500.0f);
	heightMap = new HeightMap(TEXTUREDIR "terrain.raw");
	quad = Mesh::GenerateQuad();

	camera->SetPosition(Vector3(RAW_WIDTH * HEIGHTMAP_X / 2.0f,
		500.0f, RAW_WIDTH * HEIGHTMAP_X));

	light = new Light(Vector3((RAW_HEIGHT * HEIGHTMAP_X / 2.0f), 500.0f,
		(RAW_HEIGHT * HEIGHTMAP_Z / 2.0f)),
		Vector4(0.9f, 0.9f, 1.0f, 1),
		(RAW_WIDTH * HEIGHTMAP_X) / 2.0f);

	hellData = new MD5FileData(MESHDIR "hellknight.md5mesh");
	hellNode = new MD5Node(*hellData);

	hellData->AddAnim(MESHDIR "idle2.md5anim");
	hellNode->PlayAnim(MESHDIR "idle2.md5anim");

	sceneShader = new Shader(SHADERDIR "shadowscenevert.glsl",
		SHADERDIR "shadowscenefrag.glsl");
	/*particleShader = new Shader(SHADERDIR"particleVertex.glsl", 
		SHADERDIR"particleFragment.glsl");*/
	shadowShader = new Shader(SHADERDIR "shadowVert.glsl",
		SHADERDIR "shadowFrag.glsl");

	reflectShader = new Shader(SHADERDIR "PerPixelVertex.glsl",
		SHADERDIR "reflectFragment.glsl");
	skyboxShader = new Shader(SHADERDIR "skyboxVertex.glsl",
		SHADERDIR "skyboxFragment.glsl");
	lightShader = new Shader(SHADERDIR "PerPixelVertex.glsl",
		SHADERDIR "PerPixelFragment.glsl");

	if (!reflectShader->LinkProgram() || !lightShader->LinkProgram() ||
		!skyboxShader->LinkProgram()) {
		return;
	}

	if (!sceneShader->LinkProgram() || !shadowShader->LinkProgram()) {
		return;
	}

	quad->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR "water.TGA",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	heightMap->SetTexture(SOIL_load_OGL_texture(
		TEXTUREDIR "Barren Reds.JPG", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	heightMap->SetBumpMap(SOIL_load_OGL_texture(
		TEXTUREDIR "Barren RedsDOT3.JPG", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	cubeMap = SOIL_load_OGL_cubemap(
		TEXTUREDIR "sea_ft.JPG", TEXTUREDIR "sea_bk.JPG",
		TEXTUREDIR "sea_up.JPG", TEXTUREDIR "sea_dn.JPG",
		TEXTUREDIR "sea_rt.JPG", TEXTUREDIR "sea_lf.JPG",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID, 0
	);

	if (!cubeMap || !quad->GetTexture() || !heightMap->GetTexture() ||
		!heightMap->GetBumpMap()) {
		return;
	}

	SetTextureRepeating(quad->GetTexture(), true);
	SetTextureRepeating(heightMap->GetTexture(), true);
	SetTextureRepeating(heightMap->GetBumpMap(), true);

	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
		GL_COMPARE_R_TO_TEXTURE);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &shadowFBO);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	floor = Mesh::GenerateQuad();
	floor->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR "brick.tga"
		, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	floor->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR "brickDOT3.tga"
		, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	glEnable(GL_DEPTH_TEST);

	projMatrix = Matrix4::Perspective(1.0f, 15000.0f,
		(float)width / (float)height, 45.0f);

	init = true;
	waterRotate = 0.0f;

	projMatrix = Matrix4::Perspective(1.0f, 15000.0f,
		(float)width / (float)height, 45.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

Renderer ::~Renderer(void) {
	glDeleteTextures(1, &shadowTex);
	glDeleteFramebuffers(1, &shadowFBO);

	delete camera;
	delete heightMap;
	delete quad;
	delete reflectShader;
	delete skyboxShader;
	delete lightShader;
	delete light;
	delete hellData;
	delete hellNode;
	delete floor;

	delete sceneShader;
	delete shadowShader;
	currentShader = NULL;
}

void Renderer::UpdateScene(float msec) {

	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	
	
	waterRotate += msec / 1000.0f;
	hellNode->Update(msec);
}

void Renderer::RenderScene() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	DrawSkybox();
	DrawHeightmap();
	DrawWater();
	DrawShadowScene();
	DrawCombinedScene();

	SwapBuffers();
}

void Renderer::DrawSkybox() {
	glDepthMask(GL_FALSE);
	SetCurrentShader(skyboxShader);

	UpdateShaderMatrices();
	quad->Draw();

	glUseProgram(0);
	glDepthMask(GL_TRUE);
}

void Renderer::DrawHeightmap() {
	SetCurrentShader(lightShader);
	SetShaderLight(*light);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)&camera->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"bumpTex"), 1);

	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();

	UpdateShaderMatrices();

	heightMap->Draw();

	glUseProgram(0);
}

void Renderer::DrawWater() {
	SetCurrentShader(reflectShader);
	SetShaderLight(*light);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)&camera->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"cubeTex"), 2);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

	float heightX = (RAW_WIDTH * HEIGHTMAP_X / 2.0f);

	float heightY = 256 * HEIGHTMAP_Y / 3.0f;

	float heightZ = (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f);

	modelMatrix =
		Matrix4::Translation(Vector3(heightX, heightY, heightZ)) *
		Matrix4::Scale(Vector3(heightX, 1, heightZ)) *
		Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

	textureMatrix = Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) *
		Matrix4::Rotation(waterRotate, Vector3(0.0f, 0.0f, 1.0f));

	UpdateShaderMatrices();

	quad->Draw();

	glUseProgram(0);
}

void Renderer::DrawShadowScene() {
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);

	glClear(GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	SetCurrentShader(shadowShader);

	viewMatrix = Matrix4::BuildViewMatrix(
		light->GetPosition(), Vector3(2450, 150, 3000));
	textureMatrix = biasMatrix * (projMatrix * viewMatrix);

	UpdateShaderMatrices();

	DrawFloor();

	DrawMesh();

	glUseProgram(0);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawCombinedScene() {
	SetCurrentShader(sceneShader);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"bumpTex"), 1);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"shadowTex"), 2);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)&camera->GetPosition());

	SetShaderLight(*light);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowTex);

	viewMatrix = camera->BuildViewMatrix();
	UpdateShaderMatrices();

	DrawFloor();
	DrawMesh();

	glUseProgram(0);
}

void Renderer::DrawMesh() {
	modelMatrix.ToIdentity();
	modelMatrix = Matrix4::Translation(Vector3(2450, 150, 3000));
	Matrix4 tempMatrix = textureMatrix * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram()
		, "textureMatrix"), 1, false, *&tempMatrix.values);

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram()
		, "modelMatrix"), 1, false, *&modelMatrix.values);

	hellNode->Draw(*this);
}

void Renderer::DrawFloor() {
	modelMatrix = Matrix4::Translation(Vector3(2450	,150, 3000))*Matrix4::Rotation(90, Vector3(1, 0, 0)) *
		Matrix4::Scale(Vector3(450, 450, 1));
	Matrix4 tempMatrix = textureMatrix * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram()
		, "textureMatrix"), 1, false, *&tempMatrix.values);
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram()
		, "modelMatrix"), 1, false, *&modelMatrix.values);

	floor->Draw();
}