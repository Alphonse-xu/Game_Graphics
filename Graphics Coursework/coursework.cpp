#pragma comment(lib, "nclgl.lib")

#include "../../NCLGL/window.h"
#include "Renderer.h"
#include "../../NCLGL/Shader.h"
#include "../../NCLGL/Mesh.h"


int main()	{
	Window w("My First OpenGL 3 Triangle!", 800 , 600, false); //声明nclgl Window类的实例。需要4参数-显示在窗口顶部的字符串，窗口的宽度和高度（以像素）和一个布尔值（确定是否为全屏）。
	//如果出于某种原因我们窗口无法初始化，我们通过if语句使程序退出。
	if(!w.HasInitialised()) {
		return -1;
	}
	
	Renderer renderer(w); //创建Renderer类的一个实例，将其传递给Window类，以便它知道要渲染到的内容。
	if(renderer.HasInitialised()) {
		return -1;
	}
	//一个窗口闭合（while循环中断条件的前半部分），或者用户按Escape键（后半部分）
	while(w.UpdateWindow()  && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){
		renderer.RenderScene();
	}

	return 0;
}