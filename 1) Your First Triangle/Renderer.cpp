#include "Renderer.h"

Renderer::Renderer(Window &parent) : OGLRenderer(parent)	{
	triangle = Mesh::GenerateTriangle(); //指向MESH类

	currentShader = new Shader(SHADERDIR"CourseVertexShader.glsl",
		SHADERDIR"CourseFragmentShader.glsl");

	if(!currentShader->LinkProgram()) {
		return;
	}

	init = true;
}
Renderer::~Renderer(void)	{
	delete triangle;
}

void Renderer::RenderScene()	{
	glClearColor(1.0f,1.0f,0.0f,1.0f);	//设置清屏颜色
	glClear(GL_COLOR_BUFFER_BIT);	//清屏

	//使用shader
	glUseProgram(currentShader->GetProgram()); //shader.program才是返回的句柄
	triangle->Draw();
	glUseProgram(0);

	SwapBuffers();	
}

