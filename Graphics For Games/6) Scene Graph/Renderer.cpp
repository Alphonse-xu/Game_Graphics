#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	CubeRobot::CreateCube(); // Important !
	triangle = Mesh::GenerateTriangle();
	camera = new Camera();

	currentShader = new Shader(SHADERDIR "SceneVertex.glsl",
		SHADERDIR "SceneFragment.glsl");

	tshader = new Shader(SHADERDIR"basicVertex.glsl",
		SHADERDIR"colourFragment.glsl");

	if (!currentShader->LinkProgram() || !tshader->LinkProgram()) {
		return;

	}

	projMatrix = Matrix4::Perspective(1.0f, 10000.0f,
		(float)width / (float)height, 45.0f);

	camera->SetPosition(Vector3(0, 30, 175));

	root = new SceneNode();
	root->AddChild(new CubeRobot());

	glEnable(GL_DEPTH_TEST);
	init = true;

}
Renderer ::~Renderer(void) {
	delete root;
	CubeRobot::DeleteCube(); // Also important !
	delete triangle;
}

void Renderer::UpdateScene(float msec) {
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	root->Update(msec);

}

void Renderer::RenderScene() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glUseProgram(currentShader->GetProgram());
	UpdateShaderMatrices();

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 1);

	glViewport(0, 0, 800, 600);
	glColor3f(0.0, 0.4, 0.2);//设定物体的RGB颜色
	glBegin(GL_LINES);//显示一组之弦断，其端点坐标在glVertex函数中指定
	glVertex2f(0.0, -1.0);//指定第一个点
	glVertex2f(0.0, 1.0);//指定第二个点
	glEnd();//结束
	glFlush();

	glViewport(0, 0, 400, 600);
	DrawNode(root);
	glUseProgram(0);

	glViewport(400, 0, 400, 600);
	glUseProgram(currentShader->GetProgram());
	triangle->Draw();
	glUseProgram(0);

	SwapBuffers();
}

void Renderer::DrawNode(SceneNode* n) {
	if (n->GetMesh()) {
		Matrix4 transform = n->GetWorldTransform() *
			Matrix4::Scale(n->GetModelScale());
		glUniformMatrix4fv(
			glGetUniformLocation(currentShader->GetProgram(),
				"modelMatrix"), 1, false, (float*)&transform);

		glUniform4fv(glGetUniformLocation(currentShader->GetProgram(),
			"nodeColour"), 1, (float*)&n->GetColour());

		glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
			"useTexture"), (int)n->GetMesh()->GetTexture());
		n->Draw(*this);

	}

	for (vector < SceneNode* >::const_iterator
		i = n->GetChildIteratorStart();
		i != n->GetChildIteratorEnd(); ++i) {
		DrawNode(*i);

	}

}