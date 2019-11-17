#pragma comment(lib, "nclgl.lib")

#include <iostream>
#include <cstdlib>
using std::cout;
using std::endl;

//窗口管理替代GLFW
#include "../../NCLGL/window.h"

//父类OG包含GLEW支持GL函数
#include "Renderer.h"

#include "../../NCLGL/Shader.h"
#include "../../NCLGL/Mesh.h"


int main()	{
	Window w("My First OpenGL 3 Triangle!", 800 , 600, false); //参数-显示在窗口顶部的字符串，窗口的宽度和高度（以像素）和一个布尔值(确定是否为全屏)
	if(!w.HasInitialised()) {
		cout << "Failed to creat window" << endl; //如果窗口无法初始化，终止进程
		return -1;
	}
	
	Renderer renderer(w); //创建Renderer类的一个实例，将其传递给Window类，以便它知道要渲染到的内容。
	if(!renderer.HasInitialised()) {
		cout << "Failed to init draw" << endl; //如果窗口无法初始化，终止进程
		return -1;
	}

	//窗口关闭判定（while循环中断条件的前半部分），或者用户按Escape键（后半部分）
	while(w.UpdateWindow()  && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){
		renderer.RenderScene();
	}

	//glDeleteProgram(currentShader);
	//缺少终止进程函数glfwTerminate()
	return 0;
}