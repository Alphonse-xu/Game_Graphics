#include "Renderer.h"
Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	 triangle = Mesh::GenerateTriangle();
	 camera = new Camera();
	
		 currentShader = new Shader(SHADERDIR "MatrixVertex.glsl",
			 SHADERDIR "colourFragment.glsl");
	
		 if (!currentShader -> LinkProgram()) {
		 return;
		
	}
	
		 init = true;
	
		 SwitchToPerspective();
	
}

 Renderer ::~Renderer(void) {
	 delete triangle;	
	 delete camera;
}

  void Renderer::SwitchToPerspective() {
	  projMatrix = Matrix4::Perspective(1.0f,10000.0f,(float)width / (float)height, 60.0f); //透视投影矩阵公式 近截面和远界面的深度 宽高比 视锥上下面之间的夹角
	 
 }

  void Renderer::SwitchToOrthographic() {
	   projMatrix = Matrix4::Orthographic(-1.0f,10000.0f,width/2.0f,-width / 2.0f, height/ 2.0f, -height/2.0f); //正交投影矩阵公式
	  
  }

  void Renderer::RenderScene() {

	   glClear(GL_COLOR_BUFFER_BIT);
	  
		   glUseProgram(currentShader -> GetProgram());
	  
		   glUniformMatrix4fv(glGetUniformLocation(currentShader -> GetProgram(), "projMatrix"), 1, GL_FALSE, (float*)&projMatrix);
	  //通过一致变量（uniform修饰的变量）引用将一致变量值传入渲染管线
		   glUniformMatrix4fv(glGetUniformLocation(currentShader -> GetProgram(), "viewMatrix"), 1, GL_FALSE, (float*)&viewMatrix);
	  //首先执行缩放，接着旋转，最后才是平移
		   for (int i = 0; i < 3; ++i) {
		   Vector3 tempPos = position;
		   tempPos.z += (i * 500.0f);
		   tempPos.x -= (i * 100.0f);
		   tempPos.y -= (i * 100.0f);
		  
			   modelMatrix = Matrix4::Translation(tempPos) * Matrix4::Rotation(rotation, Vector3(0, 1, 0)) * Matrix4::Scale(Vector3(scale, scale, scale));
		  
			   glUniformMatrix4fv(glGetUniformLocation(currentShader -> GetProgram(), "modelMatrix"), 1, false,(float*)&modelMatrix);
		   triangle -> Draw();
		  
	  }
	  
		   glUseProgram(0);
	  
		   SwapBuffers();
	  
  }
 
  void Renderer::UpdateScene(float msec) {
	   camera -> UpdateCamera(msec);
	   viewMatrix = camera -> BuildViewMatrix();
  }

