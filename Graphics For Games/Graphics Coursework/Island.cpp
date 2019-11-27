#include "Island.h"

Mesh* SmallIsland::Island = NULL;

SmallIsland::SmallIsland(void) {
	SetMesh(Island); 
	

}

//动画
void SmallIsland::Update(float msec) {
	transform = transform * Matrix4::Rotation(msec / 10.0f, Vector3(0, 1, 0));

	SceneNode::Update(msec);
}
