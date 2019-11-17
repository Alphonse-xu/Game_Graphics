#pragma once
#include "../nclgl/OGLRenderer.h"

class Renderer:public OGLRenderer
{
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void RenderScene();

	//纹理
	void UpdateTextureMatrix(float rotation);
	void ToggleRepeating();
	void ToggleFiltering();

protected:
	Mesh* triangle;

	//纹理
	bool filtering;
	bool repeating;
};

