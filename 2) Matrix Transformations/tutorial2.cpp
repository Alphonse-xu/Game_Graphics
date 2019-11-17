#pragma comment(lib, "nclgl.lib")

#include "../../nclGL/window.h"
#include "Renderer.h"
#include "glm/glm.hpp"

int main() {
	Window w("Vertex Transformation!", 800, 600, false);
	
	while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		Renderer renderer(w);
		if (!w.HasInitialised()) {
			return -1;
		}

		
		if (!renderer.HasInitialised()) {
			return -1;
		}

		float scale = 100.0f;
		float rotation = 0.0f;
		Vector3 position(0, 0, -1500.0f);

		while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {

			if (Window::GetKeyboard()->KeyDown(KEYBOARD_1))
				renderer.SwitchToOrthographic();
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_2))
				renderer.SwitchToPerspective();

			if (Window::GetKeyboard()->KeyDown(KEYBOARD_PLUS))
				++scale;
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_MINUS))
				--scale;

			if (Window::GetKeyboard()->KeyDown(KEYBOARD_LEFT))
				++rotation; 
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_RIGHT))
				--rotation;

			if (Window::GetKeyboard()->KeyDown(KEYBOARD_K))
				position.y -= 1.0f;
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_I))
				position.y += 1.0f;

			if (Window::GetKeyboard()->KeyDown(KEYBOARD_J))
				position.x -= 1.0f;
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_L))
				position.x += 1.0f;

			if (Window::GetKeyboard()->KeyDown(KEYBOARD_O))
				position.z -= 1.0f;
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_P))
				position.z += 1.0f;

            
			//第一次生成三角形，MVP变换得到三个三角形
			renderer.SetRotation(rotation);
			renderer.SetScale(scale);
			renderer.SetPosition(position);
			renderer.RenderScene();
			//微妙为单位刷新画面
			renderer.UpdateScene(w.GetTimer()->GetTimedMS());
			renderer.RenderScene();
		}

		return 0;
		//cout << "timer:"<<w.GetTimer()->GetTimedMS()<<endl;
	
	
	}
	

}