#pragma once
#include "../..\nclgl/Matrix4.h"
#include "../..\nclgl/Vector4.h"
#include "../..\nclgl/Vector3.h"
#include "../..\nclgl/Mesh.h"
#include <vector>

class SceneNode
{
public:
	SceneNode(Mesh* m = NULL, Vector4 colour = Vector4(1, 1, 1, 1));
	~SceneNode(void);

	void SetTransform(const Matrix4& matrix) { transform = matrix; }
	const Matrix4& GetTransform() const { return transform; }
	Matrix4 GetWorldTransform() const { return worldTransform; } 

	Vector4 GetColour() const { return colour; } 
	void SetColour(Vector4 c) { colour = c; } 

	Vector3 GetModelScale() const { return modelScale; } 
	void SetModelScale(Vector3 s) { modelScale = s; } 

	Mesh * GetMesh() const { return mesh; } 
	void SetMesh(Mesh * m) { mesh = m; } 

	void AddChild(SceneNode * s); 

	//更新资料将遍历场景图，建立世界变换并更新
	virtual void Update(float msec);
	virtual void Draw(const OGLRenderer & r);

	 std::vector <SceneNode*>::const_iterator GetChildIteratorStart() { 
		 return children.begin();
	 } 
	 std::vector <SceneNode*>::const_iterator GetChildIteratorEnd() {
		 return children.end();
	 }

protected:
	//节点父类和子类
	std::vector<SceneNode*>  children;
	SceneNode* parent;

	Mesh* mesh;
	Matrix4 worldTransform; //世界变换
	Matrix4 transform; //局部变换
	Vector3 modelScale; //模型比例尺
	Vector4 colour;
	
};

