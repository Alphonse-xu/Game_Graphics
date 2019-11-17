 #include "SceneNode.h"


SceneNode::SceneNode(Mesh * mesh, Vector4 colour) { 
	this->mesh = mesh; 
	this->colour = colour; 
	parent = NULL; 
	modelScale = Vector3(1, 1, 1); 
} 
SceneNode::~SceneNode(void) { 
	for (unsigned int i = 0; i < children.size(); ++i) { 
		delete children[i]; 
	} 
} 

//将一个SceneNode添加到其子向量中，并且还将新的子节点设置为父节点本身
void SceneNode::AddChild( SceneNode* s ) { 
	children.push_back(s); 
	s->parent = this; 
}

//如果特定的SceneNode必须执行更高级的渲染设置（禁用深度测试或重新上传数据），可以在此处完成。
void SceneNode::Draw(const OGLRenderer& r) 
{
	if (mesh) { mesh->Draw(); };
}

//产生正确的世界空间只需将节点的局部变换矩阵乘以它的父母的世界矩阵
void SceneNode::Update(float msec) {
	if (parent) { //This node has a parent...
		worldTransform = parent ->worldTransform * transform; 
	} 
	else{ //Root node , world transform is local transform! 
		worldTransform = transform; 
	} 
	for(vector <SceneNode*>::iterator i = children.begin(); 
		i != children.end(); ++i) {
		(*i)->Update(msec); 
	} 
}