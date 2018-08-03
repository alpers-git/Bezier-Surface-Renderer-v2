#pragma once
#include "SceneObject.h"

class DraggablePoint : public SceneObject
{
public:
	DraggablePoint();
	~DraggablePoint();

	void Draw(Renderer renderer, Shader* shader, VertexArray* vArray);

private:
	VertexBufferLayout m_layout;
	void PushCubeModel();

};
